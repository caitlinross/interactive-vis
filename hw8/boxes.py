#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

dim = 15
boxes = [[[0 for k in xrange(dim)] for j in xrange(dim)] for i in xrange(dim)]
for i in range(dim):
    for j in range(dim):
        for k in range(dim):
            boxes[i][j][k] = Box()
            boxes[i][j][k].Center = [i*2, j*2, k*2]

#boxes.Opacity 0..1

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')
# uncomment following to set a specific view size
# renderView1.ViewSize = [1146, 1091]

# show data in view
box1Display = Show(box1, renderView1)
# trace defaults for the display properties.
box1Display.ColorArrayName = [None, '']

# reset view to fit data
renderView1.ResetCamera()

# reset view to fit data
renderView1.ResetCamera()

# set scalar coloring
ColorBy(box1Display, ('POINTS', 'Normals'))

# rescale color and/or opacity maps used to include current data range
box1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
box1Display.SetScalarBarVisibility(renderView1, True)

# set scalar coloring
ColorBy(box1Display, ('POINTS', 'TCoords'))

# rescale color and/or opacity maps used to include current data range
box1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
box1Display.SetScalarBarVisibility(renderView1, True)

# set scalar coloring
ColorBy(box1Display, ('CELLS', 'cellNormals'))

# rescale color and/or opacity maps used to include current data range
box1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
box1Display.SetScalarBarVisibility(renderView1, True)

# get color transfer function/color map for 'cellNormals'
cellNormalsLUT = GetColorTransferFunction('cellNormals')

# get opacity transfer function/opacity map for 'cellNormals'
cellNormalsPWF = GetOpacityTransferFunction('cellNormals')

# set scalar coloring
ColorBy(box1Display, ('POINTS', 'TCoords'))

# rescale color and/or opacity maps used to include current data range
box1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
box1Display.SetScalarBarVisibility(renderView1, True)

# set scalar coloring
ColorBy(box1Display, ('POINTS', 'Normals'))

# rescale color and/or opacity maps used to include current data range
box1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
box1Display.SetScalarBarVisibility(renderView1, True)

# turn off scalar coloring
ColorBy(box1Display, None)

# set scalar coloring
ColorBy(box1Display, ('POINTS', 'Normals'))

# rescale color and/or opacity maps used to include current data range
box1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
box1Display.SetScalarBarVisibility(renderView1, True)

# turn off scalar coloring
ColorBy(box1Display, None)

# set scalar coloring
ColorBy(box1Display, ('POINTS', 'Normals'))

# rescale color and/or opacity maps used to include current data range
box1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
box1Display.SetScalarBarVisibility(renderView1, True)

# turn off scalar coloring
ColorBy(box1Display, None)

#### saving camera placements for all active views

# current camera placement for renderView1
renderView1.CameraPosition = [-20.868560838007475, 8.147670172689686, 2.201854488147358]
renderView1.CameraFocalPoint = [9.999999999999993e-21, 2.1499681204485614e-36, 7.917671058959606e-37]
renderView1.CameraViewUp = [0.11861291386136075, 0.03563950385203123, 0.992300762082998]
renderView1.CameraParallelScale = 0.8660254037844386

#### uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).
