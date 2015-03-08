import os
import math
import numpy as np
from PlummerGalaxy import PlummerGalaxy
from plot_or_make_video import MakeVideo
import imp
try:
    imp.find_module('matplotlib')
    matplotlibAvailable = True
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
except ImportError:
    matplotlibAvailable = False


# Settings:

createNewInitialConditions = True

MakePositionsVideo = False
MakeDistributionsVideo = False

galaxyNumPts = 2048

#=================================================================================
if createNewInitialConditions:
	
	# Generate Plummer galaxy
	newGalaxy = PlummerGalaxy()
	newGalaxy.npts = galaxyNumPts
	newGalaxy.R = 1.0
	newGalaxy.timestep = 0.1
	newGalaxy.timemax = 12.0
	newGalaxy.ZeroVelocities_Bool = False
	newGalaxy.Aarseth_eta = 0.05
	newGalaxy.Aarseth_eps_sqd = 0.05
	
	newGalaxy.GenerateInitialConditions(0,0,0)
	newGalaxy.WriteToFile("plummer.data")
	
	print("compiling Aarseth c code...")
	os.system("(cd NBodySim_Aarseth && make)")
    	
	print("Running compiled Aarseth nbody code on Plummer initial conditions file")
	os.system("./NBodySim_Aarseth/aarseth plummer.data")


if matplotlibAvailable and (MakePositionsVideo or MakeDistributionsVideo):
	
	print("beginning to make plots/video...")	
	
	if MakePositionsVideo:
		MakeVideo(galaxyNumPts, "out_aarseth_npts_"+str(galaxyNumPts)+".data", "video_aarseth_positions.avi", True)
	if MakeDistributionsVideo:
		MakeVideo(galaxyNumPts, "out_aarseth_npts_"+str(galaxyNumPts)+".data", "video_aarseth_pos_distribution.avi", False)


