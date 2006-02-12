#Change PROJECT_NAME to the name of your project
PROJECT(projection)

# set the name of the input image used to run the test
SET(INPUT_IMAGE ${CMAKE_SOURCE_DIR}/images/ESCells.img)
SET(BINARY_IMAGE ${CMAKE_SOURCE_DIR}/images/2th_ESCells.img)

#include some macros from another file...
INCLUDE(IJMacros.txt)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)
FIND_PROGRAM(IMAGE_COMPARE ImageCompare)

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
  ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
  ITKCommon
  ITKBasicFilters
  ITKIO
)

OPTION(BUILD_WRAPPERS "Wrap library" OFF)
IF(BUILD_WRAPPERS)
   SUBDIRS(Wrapping)
ENDIF(BUILD_WRAPPERS)


# install devel files
FOREACH(f itkMaximumProjectionImageFilter.h itkMedianProjectionImageFilter.h itkProjectionImageFilter.h    itkSumProjectionImageFilter.h itkMeanProjectionImageFilter.h itkMinimumProjectionImageFilter.h itkProjectionImageFilter.txx)
  INSTALL_FILES(/include/InsightToolkit/BasicFilters FILES ${CMAKE_CURRENT_SOURCE_DIR}/${f})
ENDFOREACH(f)


#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project

SET(CurrentExe "max")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "min")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "sum")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "mean")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "median")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "sigma")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "accumulate")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "perf")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "maxdim")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

ADD_TEST(Maximum max ${INPUT_IMAGE} maximum.tif)
ADD_TEST(MaximumCompareImage ${IMAGE_COMPARE} maximum.tif ${CMAKE_SOURCE_DIR}/images/maximum.tif)

ADD_TEST(Minimum min ${INPUT_IMAGE} minimum.tif)
ADD_TEST(MinimumCompareImage ${IMAGE_COMPARE} minimum.tif ${CMAKE_SOURCE_DIR}/images/minimum.tif)

ADD_TEST(Sum sum ${INPUT_IMAGE} sum.tif)
ADD_TEST(SumCompareImage ${IMAGE_COMPARE} sum.tif ${CMAKE_SOURCE_DIR}/images/sum.tif)

ADD_TEST(Mean mean ${INPUT_IMAGE} mean.tif)
ADD_TEST(MeanCompareImage ${IMAGE_COMPARE} mean.tif ${CMAKE_SOURCE_DIR}/images/mean.tif)

ADD_TEST(Median median ${INPUT_IMAGE} median.tif)
ADD_TEST(MedianCompareImage ${IMAGE_COMPARE} median.tif ${CMAKE_SOURCE_DIR}/images/median.tif)

ADD_TEST(Sigma sigma ${INPUT_IMAGE} sigma.tif)
ADD_TEST(SigmaCompareImage ${IMAGE_COMPARE} sigma.tif ${CMAKE_SOURCE_DIR}/images/sigma.tif)

ADD_TEST(Binary100 binary ${BINARY_IMAGE} binary100.tif 100 255)
ADD_TEST(Binary100CompareImage ${IMAGE_COMPARE} binary100.tif ${CMAKE_SOURCE_DIR}/images/binary100.tif)

ADD_TEST(Binary200 binary ${BINARY_IMAGE} binary200.tif 200 0)
ADD_TEST(Binary200CompareImage ${IMAGE_COMPARE} binary200.tif ${CMAKE_SOURCE_DIR}/images/binary200.tif)


# sum is not produced with the same output type and so can't be compared
# ADD_TEST(AccumulateSum accumulate 0 ${INPUT_IMAGE} accumulate-sum.tif)
# ADD_TEST(AccumulateSumCompareImage ${IMAGE_COMPARE} accumulate-sum.tif ${CMAKE_SOURCE_DIR}/images/sum.tif)
# 
ADD_TEST(AccumulateMean accumulate 1 ${INPUT_IMAGE} accumulate-mean.tif)
ADD_TEST(AccumulateMeanCompareImage ${IMAGE_COMPARE} accumulate-mean.tif ${CMAKE_SOURCE_DIR}/images/mean.tif)


ADD_TEST(Maximum0 maxdim 0 ${INPUT_IMAGE} maximum0.tif)
ADD_TEST(Maximum0CompareImage ${IMAGE_COMPARE} maximum0.tif ${CMAKE_SOURCE_DIR}/images/maximum0.tif)

ADD_TEST(Maximum1 maxdim 1 ${INPUT_IMAGE} maximum1.tif)
ADD_TEST(Maximum1CompareImage ${IMAGE_COMPARE} maximum1.tif ${CMAKE_SOURCE_DIR}/images/maximum1.tif)

ADD_TEST(Maximum2 maxdim 2 ${INPUT_IMAGE} maximum2.tif)
ADD_TEST(Maximum2CompareImage ${IMAGE_COMPARE} maximum2.tif ${CMAKE_SOURCE_DIR}/images/maximum.tif)
