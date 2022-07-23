struct jointdef
{
	float mat[4][4];
	float mn, mx;
	int type; //0-fixed, 1-prismatic, 2-revolute
};

struct solution
{
	int degrees;
	float values[16]{};
};

extern "C" __declspec(dllexport) int InverseKinematics(void* pbuf, float mat[4][4], solution * q);
extern "C" __declspec(dllexport) int GetBufferSize(void);
extern "C" __declspec(dllexport) int GetNumSolutions(void);
extern "C" __declspec(dllexport) int GetDegreesOfFreedom(void);
extern "C" __declspec(dllexport) int ComputeParameters(void *pbuf,jointdef *joints,int numjoints);
