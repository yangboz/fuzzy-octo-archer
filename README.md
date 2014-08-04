fuzzy-octo-archer
=================

A Review on Feature Extraction Techniques as following:

###History:

EigenFace->FisherFaces->HAAR/LBP(ChiSquareDistance/k-NN)->SVM->...

###Face Features in kind of techniques:

#### iOS CoreImage related:

###### CGRect bounds;BOOL hasLeftEyePosition;CGPoint leftEyePosition;CGPoint rightEyePosition;BOOL hasMouthPosition;CGPoint mouthPosition;BOOL hasTrackingID;int trackingID;BOOL hasTrackingFrameCount;int trackingFrameCount;BOOL hasFaceAngle;float faceAngle;BOOL hasSmile;BOOL leftEyeClosed;BOOL rightEyeClosed;

#### OpenCV HAAR related:

###### haarcascade_frontalface_alt_tree.xml;
######

#### OpenCV LBP related:

Technical notes
=================

AAM and ASM usually used for finding facial feature points, and they need already defined face region. 

Face Database
=================


References
=================

OpenCV: http://opencv.org/ Code: https://github.com/MasteringOpenCV

FaceTracker: http://facetracker.net/

FaceL: http://sourceforge.net/projects/pyvision/

VOSM: https://github.com/jiapei100/VOSM ; http://www.visionopen.com/downloads/open-source-software/vosm/

ASM: http://code.google.com/p/asmlib-opencv/

AAM: https://github.com/jiapei100/VOSM

ChessBoard: http://docs.opencv.org/doc/tutorials/calib3d/camera_calibration_square_chess/camera_calibration_square_chess.html

FaceOSC is a tool for prototyping face-based interaction. It's built on non-commercial open source FaceTracker code from Jason Saragih. http://vimeo.com/26098366

FlandMark: http://cmp.felk.cvut.cz/~uricamic/flandmark/ 

###Demos

####The input and output DEMO on auto aging face:

![Screenshot of "Find Face"](https://raw.githubusercontent.com/yangboz/fuzzy-octo-archer/master/snapshots/DEMO_Find_face.png)

![Screenshot of "Verify Face"](https://raw.githubusercontent.com/yangboz/fuzzy-octo-archer/master/snapshots/DEMO_vFace.png)

![Screenshot of "Aging Face"](https://raw.githubusercontent.com/yangboz/fuzzy-octo-archer/master/snapshots/DEMO_FaceAging.png)

####The input and output DEMO on AI Go judge:

####The input and output DEMO on FaceFeatures(ASM/AAM+OpenCV+Boost/CoreImage(iOS)):
