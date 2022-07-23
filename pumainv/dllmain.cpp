#include <windows.h>
#include <math.h>
#include <memory>
#include "inv.h"

#define EPS 1.0e-5F
#define CHECK(var,val) if ( (float)fabs(var-(val)) > EPS) return 0
#define CHECKTRIG(x) if (fabs(x)>1.) return 0

#define RPI 3.14159265358979323846   // pi
static constexpr float RAD2DEG(float rad)
{
	return (float)(rad * 180.0 / RPI);
}

/*
	Sequence of operation -
	1. Call GetBufferSize to find out size of buffer necessary
	2. allocate buffer of correct size and store in agent
	3. Call Compute Parameters to fill out buffer
	4. Call InverseKinematics to get solution
*/

struct invbuf
{
	float a2,a3;
	float d2,d4,d6;
	float pre[4][4];
	float post[4][4];
};
#define A2 pbuf->a2
#define A3 pbuf->a3
#define D2 pbuf->d2
#define D4 pbuf->d4
#define D6 pbuf->d6

static float ident[4][4] =
{
	{1.F,0.F,0.F,0.F},
	{0.F,1.F,0.F,0.F},
	{0.F,0.F,1.F,0.F},
	{0.F,0.F,0.F,1.F}
};

static int ComputeTheta1(float theta[6],float p[3],struct invbuf *pbuf,float ARM);
static int ComputeTheta2(float theta[6],float p[3],struct invbuf *pbuf,float ARM,float ELBOW);
static int ComputeTheta3(float theta[6],float p[3],struct invbuf *pbuf,float ARM,float ELBOW);
static int ComputeTheta4(float theta[6],float a[3],struct invbuf *pbuf,float WRIST);
static int ComputeTheta5(float theta[6],float a[3]);
static int ComputeTheta6(float theta[6],float n[3],float s[3]);
static int GetDH(float m[4][4],float *ptheta,float *pdz,float *pda,float *palpha);
static void fastinverse(float m[4][4]);
static void mult44(float res[4][4],float m[4][4],float mat[4][4]);
static int IK(void *pbuf,float mat[4][4],solution* q);

int InverseKinematics(void *pbuf,float mat[4][4],solution* q)
{
	float tmat[4][4];
	struct invbuf *buf = (struct invbuf *)pbuf;
	mult44(tmat,buf->pre,mat);
	mult44(tmat,tmat,buf->post);
	return IK(pbuf,tmat,q);
}

static int IK(void *pbuf,float mat[4][4],solution* q)
{
	float p[3];
	float n[3] = {mat[0][0],mat[1][0],mat[2][0]};
	float s[3] = {mat[0][1],mat[1][1],mat[2][1]};
	float a[3] = {mat[0][2],mat[1][2],mat[2][2]};
	float p6[3] = {mat[0][3],mat[1][3],mat[2][3]};
	float arm,elbow,wrist;
	struct invbuf *buf = (struct invbuf *)pbuf;
	float d6 = buf->d6;
	int i,j;

	arm = elbow = wrist = -1.F;
	p[0] = p6[0] - d6*a[0];
	p[1] = p6[1] - d6*a[1];
	p[2] = p6[2] - d6*a[2];
	for (i=0;i<8;i++)
	{
		wrist = (i&1) ? 1.F : -1.F;
		elbow = (i&2) ? 1.F : -1.F;
		arm = (i&4) ? 1.F : -1.F;
		if (!ComputeTheta1(q[i].values,p,buf,arm))
			return 0;
		if (!ComputeTheta2(q[i].values,p,buf,arm,elbow))
			return 0;
		if (!ComputeTheta3(q[i].values,p,buf,arm,elbow))
			return 0;
		if (!ComputeTheta4(q[i].values,a,buf,wrist))
			return 0;
		if (!ComputeTheta5(q[i].values,a))
			return 0;
		if (!ComputeTheta6(q[i].values,n,s))
			return 0;
		for (j=0;j<6;j++)
			q[i].values[j] = RAD2DEG(q[i].values[j]);
	}
	return 1;
}

static int ComputeTheta1(float theta[6],float p[3],struct invbuf *pbuf,float ARM)
{
	float t = p[0]*p[0]+p[1]*p[1]-D2*D2;
	if (t < 0.F)
		return 0;
	t = (float)sqrt(t);
	theta[0] = (float)atan2(-ARM*p[1]*t - p[0]*D2,-ARM*p[0]*t + p[1]*D2);
	return 1;
}

static int ComputeTheta2(float theta[6],float p[3],struct invbuf *pbuf,float ARM,float ELBOW)
{
	float bigr;
	float r;
	float sina,cosa,sinb,cosb,st,ct;

	bigr = p[0]*p[0]+p[1]*p[1]+p[2]*p[2]-D2*D2;
	r = p[0]*p[0]+p[1]*p[1]-D2*D2;
	if (bigr <= 0.F)
		return 0;
	bigr = (float)sqrt(bigr);
	if (r < 0.F)
		return 0; 
	r = (float)sqrt(r);
	sina = -p[2]/bigr;
	cosa = -ARM*r/bigr;
	CHECKTRIG(sina);
	CHECKTRIG(cosa);
	cosb = (A2*A2 + bigr*bigr - D4*D4 - A3*A3)/(2.F*A2*bigr);
	CHECKTRIG(cosb);
	sinb = (float)sqrt(1.-cosb*cosb);

	st = sina*cosb+ARM*ELBOW*cosa*sinb;
	ct = cosa*cosb-ARM*ELBOW*sina*sinb;
	theta[1] = (float) atan2(st,ct);
	return 1;
}

static int ComputeTheta3(float theta[6],float p[3],struct invbuf *pbuf,float ARM,float ELBOW)
{
	float bigr;
	float sinp,cosp,sinb,cosb,st,ct;

	bigr = p[0]*p[0]+p[1]*p[1]+p[2]*p[2]-D2*D2;
	if (bigr < 0.F)
		return 0;
	bigr = (float)sqrt(bigr);

	cosp = (A2*A2 + D4*D4 + A3*A3 - bigr*bigr)/(2.F * A2 * (float)sqrt(D4*D4+A3*A3));
	sinp = ARM*ELBOW*(float)sqrt(1.-cosp*cosp);
	cosb = (float)(fabs(A3)/sqrt(D4*D4+A3*A3));
	sinb = D4/(float)sqrt(D4*D4+A3*A3);
	CHECKTRIG(sinp);
	CHECKTRIG(cosp);
	CHECKTRIG(sinb);
	CHECKTRIG(cosb);

	st = sinp*cosb-cosp*sinb;
	ct = cosp*cosb+sinp*sinb;
	theta[2] = (float) atan2(st,ct);
	return 1;
}

static int ComputeTheta4(float theta[6],float a[3],struct invbuf *pbuf,float WRIST)
{
	float st,ct,s1,c1,s23,c23;
	s1 = (float)sin(theta[0]);
	c1 = (float)cos(theta[0]);
	s23 = (float)sin(theta[1]+theta[2]);
	c23 = (float)cos(theta[1]+theta[2]);
	st = WRIST*(c1*a[1]-s1*a[0]);
	ct = WRIST*(c1*c23*a[0]+s1*c23*a[1]-s23*a[2]);
	theta[3] = (float) atan2(st,ct);
	return 1;
}

static int ComputeTheta5(float theta[6],float a[3])
{
	float st,ct,s1,c1,s23,c23,s4,c4;
	s1 = (float)sin(theta[0]);
	c1 = (float)cos(theta[0]);
	s23 = (float)sin(theta[1]+theta[2]);
	c23 = (float)cos(theta[1]+theta[2]);
	s4 = (float)sin(theta[3]);
	c4 = (float)cos(theta[3]);

	st = (c1*c23*c4-s1*s4)*a[0]+(s1*c23*c4+c1*s4)*a[1]-c4*s23*a[2];
	ct = c1*s23*a[0]+s1*s23*a[1]+c23*a[2];
	theta[4] = (float) atan2(st,ct);
	return 1;
}

static int ComputeTheta6(float theta[6],float n[3],float s[3])
{
	float st,ct,s1,c1,s23,c23,s4,c4;
	s1 = (float)sin(theta[0]);
	c1 = (float)cos(theta[0]);
	s23 = (float)sin(theta[1]+theta[2]);
	c23 = (float)cos(theta[1]+theta[2]);
	s4 = (float)sin(theta[3]);
	c4 = (float)cos(theta[3]);

	st = (-s1*c4-c1*c23*s4)*n[0]+(c1*c4-s1*c23*s4)*n[1]+s4*s23*n[2];
	ct = (-s1*c4-c1*c23*s4)*s[0]+(c1*c4-s1*c23*s4)*s[1]+s4*s23*s[2];
	theta[5] = (float) atan2(st,ct);
	return 1;
}

int GetBufferSize(void)
{
	return sizeof(struct invbuf);
}

int GetNumSolutions(void)
{
	return 8;
}

int GetDegreesOfFreedom(void)
{
	return 6;
}

int ComputeParameters(void *pbuf,jointdef *joints,int numjoints)
{
	int i;
	int precnt=0,postcnt=0,movcnt=0;
	float theta,dz,da,alpha;
	struct invbuf *buf = (struct invbuf *)pbuf;
	memcpy(buf->pre,ident,sizeof(float)*16);
	memcpy(buf->post,ident,sizeof(float)*16);
	for (i=0;i<numjoints;i++)
	{
		if (!joints[i].type)
		{
			if (movcnt)
				postcnt++;
			else
				precnt++;
		}
		else
		{
			if (postcnt)
				return 0;
			else
				movcnt++;
		}
	}
	if (movcnt != 6)
		return 0;
	for (i=0;i<precnt;i++)
		mult44(buf->pre,buf->pre,joints[i].mat);
	for (i=precnt+movcnt;i<precnt+movcnt+postcnt;i++)
		mult44(buf->post,buf->post,joints[i].mat);
	fastinverse(buf->pre);
	fastinverse(buf->post);

	if (!GetDH(joints[precnt].mat,&theta,&dz,&da,&alpha))
		return 0;
	CHECK(theta,0.F);
	CHECK(dz,0.F);
	CHECK(da,0.F);
	CHECK(alpha,-90.F);

	if (!GetDH(joints[precnt+1].mat,&theta,&dz,&da,&alpha))
		return 0;
	CHECK(theta,0.F);
	buf->d2 = dz;
	buf->a2 = da;
	CHECK(alpha,0.F);

	if (!GetDH(joints[precnt+2].mat,&theta,&dz,&da,&alpha))
		return 0;
	CHECK(theta,0.F);
	CHECK(dz,0.F);
	buf->a3 = da;
	CHECK(alpha,90.F);

	if (!GetDH(joints[precnt+3].mat,&theta,&dz,&da,&alpha))
		return 0;
	CHECK(theta,0.F);
	buf->d4 = dz;
	CHECK(da,0.F);
	CHECK(alpha,-90.F);

	if (!GetDH(joints[precnt+4].mat,&theta,&dz,&da,&alpha))
		return 0;
	CHECK(theta,0.F);
	CHECK(dz,0.F);
	CHECK(da,0.F);
	CHECK(alpha,90.F);

	if (!GetDH(joints[precnt+5].mat,&theta,&dz,&da,&alpha))
		return 0;
	CHECK(theta,0.F);
	buf->d6 = dz;
	CHECK(da,0.F);
	CHECK(alpha,0.F);

	return 1;
}

static int GetDH(float m[4][4],float *ptheta,float *pdz,float *pda,float *palpha)
{
	if (m[0][0] > m[1][0]) // cos(theta) > sin(theta)
		*pda = m[0][3]/m[0][0];
	else
		*pda = m[1][3]/m[1][0];
	*pdz = m[2][3];

	if (fabs(m[2][0]) > EPS) // Rotation about Y
		return 0;
	if (fabs(m[0][0]*m[1][3]-m[1][0]*m[0][3]) > EPS) // translation along Y
		return 0;
	*ptheta = RAD2DEG((float)atan2(m[1][0],m[0][0]));
	*palpha = RAD2DEG((float)atan2(m[2][1],m[2][2]));
	return 1;
}

static void fastinverse(float m[4][4])
{
	int i,j;
	float t[4][4];
	memcpy(t,ident,sizeof(float)*16);
	for (i=0;i<3;i++)
		for (j=0;j<3;j++)
		{
			t[i][j] = m[j][i];
			t[j][3] = -(m[0][j]*m[0][3]+m[1][j]*m[1][3]+m[2][j]*m[2][3]);
		}
	memcpy(m,t,sizeof(float)*16);
}

static void mult44(float res[4][4],float m[4][4],float mat[4][4])
{
	int i,j;
	float t[4][4];
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
			t[i][j] = m[i][0]*mat[0][j]+
					m[i][1]*mat[1][j]+
					m[i][2]*mat[2][j]+
					m[i][3]*mat[3][j];
	memcpy(res,t,sizeof(float)*16);
}
