# Building Help for Robosim
To build the help, you need to download "HTML Help Workshop". It doesn't seem to be available on Microsoft's website any longer, but you can find a download if you search.

Launch HTML Help Workshop and select the File|Compile menu item. Specify the robo.hhp file in this directory and press the compile button. This will generate the robo.chm file, which can be checked in.

Copy robo.chm and robo.chw to the samples directory to get it work directly.

To add a new topic, create a new htm file in help\html. Add the file to the VS project. Add a section to robo.hhk. Run HTML workshop, compile the robo.hhp file. Then copy robo.chm and robo.chw to the samples directory. Run robosim, select help, and the new content should be there.