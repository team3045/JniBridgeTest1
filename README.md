Set Project Properties:

Linker / General / Additional Library Directories
C:\Program Files\Java\jdk1.7.0_51\lib

(or whatever path is appropriate for jdk / jre)

Linker / Input / Additional Dependencies
jvm.lib

- do not be surprised with the following build warnings:
----
------ Build started: Project: JniBridgeTest2, Configuration: Debug Win32 ------
  JniBridgeTest.cpp
c:\program files\java\jdk1.7.0_51\include\jni.h(1513): warning C4793: 'JNIEnv_::CallStaticVoidMethod' : function compiled as native :
  	varargs not supported under /clr
c:\program files\java\jdk1.7.0_51\include\jni.h(1515): warning C4793: 'JNIEnv_::CallStaticVoidMethod' : function compiled as native :
  	Found an intrinsic not supported in managed code
c:\program files\java\jdk1.7.0_51\include\jni.h(1517): warning C4793: 'JNIEnv_::CallStaticVoidMethod' : function compiled as native :
  	Found an intrinsic not supported in managed code
c:\program files\java\jdk1.7.0_51\include\jni.h(1352): warning C4793: 'JNIEnv_::CallStaticObjectMethod' : function compiled as native :
  	varargs not supported under /clr
c:\program files\java\jdk1.7.0_51\include\jni.h(1355): warning C4793: 'JNIEnv_::CallStaticObjectMethod' : function compiled as native :
  	Found an intrinsic not supported in managed code
c:\program files\java\jdk1.7.0_51\include\jni.h(1357): warning C4793: 'JNIEnv_::CallStaticObjectMethod' : function compiled as native :
  	Found an intrinsic not supported in managed code
c:\program files\java\jdk1.7.0_51\include\jni.h(1051): warning C4793: 'JNIEnv_::CallVoidMethod' : function compiled as native :
  	varargs not supported under /clr
c:\program files\java\jdk1.7.0_51\include\jni.h(1053): warning C4793: 'JNIEnv_::CallVoidMethod' : function compiled as native :
  	Found an intrinsic not supported in managed code
c:\program files\java\jdk1.7.0_51\include\jni.h(1055): warning C4793: 'JNIEnv_::CallVoidMethod' : function compiled as native :
  	Found an intrinsic not supported in managed code
JniBridgeTest.obj : warning LNK4248: unresolved typeref token (0100000F) for '_jmethodID'; image may not run
  JniBridgeTest.vcxproj -> C:\Documents and Settings\Administrator\My Documents\Visual Studio 2010\Projects\JniBridgeTest2\Debug\JniBridgeTest2.exe
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
----