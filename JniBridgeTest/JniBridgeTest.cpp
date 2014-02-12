// JniBridgeTest.cpp : main project file.


// plagiarized from http://stackoverflow.com/questions/9999913/calling-java-methods-from-visual-c-c-using-c-cli
// This is the main DLL file.
// see also http://www.inonit.com/cygwin/jni/invocationApi/

#include "stdafx.h"
#include <jni_md.h>
#include <jni.h>

//#include "atlstr.h"

#include <assert.h>

#pragma once

using namespace System;
//using namespace NetworkTable;

JavaVM *jvm = NULL;       /* denotes a Java VM */
JNIEnv *env = NULL;       /* pointer to native method interface */

// per http://saherneklawy.wordpress.com/2009/10/31/converting-systemstring-to-char/
void string2charPtr(String ^orig, char *&out)
{
	int length = orig->Length;
	out = new char[length+1];
	for(int i=0;i<length;i++)
		out[i] = (char) orig[i];
	out[length] = 0; // null termination
}

void invoke_class(JNIEnv* env) {
	jclass helloWorldClass;
	jmethodID mainMethod;
	jobjectArray applicationArgs;
	jstring applicationArg0;

	helloWorldClass = env->FindClass("example/jni/InvocationHelloWorld");
	assert(helloWorldClass != 0);
	
	mainMethod = env->GetStaticMethodID(helloWorldClass, "main", "([Ljava/lang/String;)V");
	assert(mainMethod != 0);

	applicationArgs = env->NewObjectArray(1, env->FindClass("java/lang/String"), NULL);
	applicationArg0 = env->NewStringUTF("From-C-program");
	env->SetObjectArrayElement(applicationArgs, 0, applicationArg0);

	env->CallStaticVoidMethod(helloWorldClass, mainMethod, applicationArgs);
}

static int createJVM( void ) {
	int errReturn = -1;
	//	JavaVM *jvm;       /* denotes a Java VM */
	//	JNIEnv *env;       /* pointer to native method interface */
	JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */

#if 0
	JavaVMOption* options = new JavaVMOption[1];
	options[0].optionString = "-Djava.class.path=c:\\projects\\local\\inonit\\classes";
	//options[0].optionString = "-Djava.class.path=/usr/lib/java";
#endif

#if 1
	JavaVMOption* options = new JavaVMOption[3];
//	options[0].optionString = "-Djava.class.path=c:\\projects\\local\\inonit\\classes";
	options[0].optionString = "-Djava.class.path=c:/WindRiver/WPILib/desktop-lib/networktables-desktop.jar";
	options[1].optionString = "-verbose:jni";                   /* print JNI-related messages */
	options[2].optionString = "-verbose:class";                   /* print classloader-related messages */
#endif

#if 0
	JavaVMOption* options = new JavaVMOption[4];
	options[0].optionString = "-Djava.compiler=NONE";           /* disable JIT */
	options[1].optionString = "-Djava.class.path=c:\myclasses"; /* user classes */
	options[2].optionString = "-Djava.library.path=c:\mylibs";  /* set native library path */
	options[3].optionString = "-verbose:jni";                   /* print JNI-related messages */
#endif

	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 1;
	vm_args.options = options;
	vm_args.ignoreUnrecognized = false;

	/* load and initialize a Java VM, return a JNI interface
	* pointer in env */
	int ret = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
	assert(ret >= 0);

	delete options;

	if (ret >= 0)
		errReturn = 0;	// success result

	return errReturn;
}

static int networkTableSetClientMode( JNIEnv* env ) {
	int errReturn = -1;

	jclass networkTableClass;
	jmethodID setClientMode;

	networkTableClass = env->FindClass("edu/wpi/first/wpilibj/networktables/NetworkTable");
	assert(networkTableClass != 0);
	
	setClientMode = env->GetStaticMethodID(networkTableClass, "setClientMode", "()V");
	assert(setClientMode != 0);

	env->CallStaticVoidMethod(networkTableClass, setClientMode, 0);

	errReturn = 0;
	return errReturn;
}

static int networkTableSetIPAddress( JNIEnv* env, const char * ipAddress ) {
	int errReturn = -1;

	jclass networkTableClass;
	jmethodID setIPAddress;

	networkTableClass = env->FindClass("edu/wpi/first/wpilibj/networktables/NetworkTable");
	assert(networkTableClass != 0);
	
	setIPAddress = env->GetStaticMethodID(networkTableClass, "setIPAddress", "(Ljava/lang/String;)V");
	assert(setIPAddress != 0);

	jstring jipAddress = env->NewStringUTF( ipAddress ); 
	env->CallStaticVoidMethod(networkTableClass, setIPAddress, jipAddress);

	errReturn = 0;
	return errReturn;
}

static jobject networkTableGetTable( JNIEnv* env, const char * tableName ) {
	int errReturn = -1;

	jclass networkTableClass;
	jmethodID getTable;
	jobject networkTableObjRef = NULL;

	networkTableClass = env->FindClass("edu/wpi/first/wpilibj/networktables/NetworkTable");
	assert(networkTableClass != 0);
	
	getTable = env->GetStaticMethodID(networkTableClass, "getTable", "(Ljava/lang/String;)Ledu/wpi/first/wpilibj/networktables/NetworkTable;");
	assert(getTable != 0);

	jstring jtableName = env->NewStringUTF( tableName ); 
	networkTableObjRef = (jobject) env->CallStaticObjectMethod(networkTableClass, getTable, jtableName);
	assert(networkTableObjRef != NULL);

	errReturn = 0;
	return networkTableObjRef;
}

static int networkTablePutNumber( JNIEnv* env, jobject networkTable, char* itemName, double n ) {
	int errReturn = -1;

	jclass networkTableClass;
	jmethodID putNumber;

	// Get a class reference for this object
	networkTableClass = env->GetObjectClass(networkTable);
	assert(networkTableClass != 0);
 
	// Get the Method ID for method "callback", which takes no arg and return void
	putNumber = env->GetMethodID(networkTableClass, "putNumber", "(Ljava/lang/String;D)V");
	assert(putNumber != 0);

	jstring jitemName = env->NewStringUTF( itemName ); 
	// jdouble doubleArgument = 9.8765;
	jdouble doubleArgument = n;
	env->CallVoidMethod(networkTable, putNumber, jitemName, doubleArgument);

	errReturn = 0;
	return errReturn;
}

static int networkTablePutString( JNIEnv* env, jobject networkTable, char* itemName, String^ itemString ) {
	int errReturn = -1;

#if 1
	jclass networkTableClass;
	jmethodID putString;

	// Get a class reference for this object
	networkTableClass = env->GetObjectClass(networkTable);
	assert(networkTableClass != 0);
 
	// Get the Method ID for method "callback", which takes no arg and return void
	putString = env->GetMethodID(networkTableClass, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
	assert(putString != 0);

	jstring jitemName = env->NewStringUTF( itemName );
	char* cItemString = NULL;
	//MarshalString(itemString , cItemString );
	string2charPtr(itemString, cItemString);
	jstring jitemString = env->NewStringUTF( cItemString );
	delete cItemString; cItemString = NULL;
	env->CallVoidMethod(networkTable, putString, jitemName, jitemString);
#endif

	errReturn = 0;
	return errReturn;
}

static int invokeMagic( JNIEnv* env ) {
	int errReturn = -1;

	jclass helloWorldClass;
	jmethodID mainMethod;
	jobjectArray applicationArgs;
	jstring applicationArg0;

	helloWorldClass = env->FindClass("example/jni/InvocationHelloWorld");
	assert(helloWorldClass != 0);
	
	mainMethod = env->GetStaticMethodID(helloWorldClass, "main", "([Ljava/lang/String;)V");
	assert(mainMethod != 0);

	applicationArgs = env->NewObjectArray(1, env->FindClass("java/lang/String"), NULL);
	applicationArg0 = env->NewStringUTF("From-C-program");
	env->SetObjectArrayElement(applicationArgs, 0, applicationArg0);

	env->CallStaticVoidMethod(helloWorldClass, mainMethod, applicationArgs);

	errReturn = 0;
	return errReturn;
}

static int releaseJVM( void ) {
	int errReturn = -1;

	jvm->DestroyJavaVM();
	jvm = NULL;
	env = NULL;

	errReturn = 0;
	return errReturn;
}

static int CreateInvokeAndReleaseVM1(void) {
	int err = 0;

	err = createJVM();

	err = invokeMagic( env );

	err = releaseJVM();

	return err;
}

static int CreateInvokeAndReleaseVM(void) {
//	JavaVM *jvm;       /* denotes a Java VM */
//	JNIEnv *env;       /* pointer to native method interface */
	JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */

#if 0
	JavaVMOption* options = new JavaVMOption[1];
	options[0].optionString = "-Djava.class.path=c:\\projects\\local\\inonit\\classes";
	//options[0].optionString = "-Djava.class.path=/usr/lib/java";
#endif

#if 1
	JavaVMOption* options = new JavaVMOption[3];
	options[0].optionString = "-Djava.class.path=c:\\projects\\local\\inonit\\classes";
	options[1].optionString = "-verbose:jni";                   /* print JNI-related messages */
	options[2].optionString = "-verbose:class";                   /* print classloader-related messages */
#endif

#if 0
	JavaVMOption* options = new JavaVMOption[4];
	options[0].optionString = "-Djava.compiler=NONE";           /* disable JIT */
	options[1].optionString = "-Djava.class.path=c:\myclasses"; /* user classes */
	options[2].optionString = "-Djava.library.path=c:\mylibs";  /* set native library path */
	options[3].optionString = "-verbose:jni";                   /* print JNI-related messages */
#endif

	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 1;
	vm_args.options = options;
	vm_args.ignoreUnrecognized = false;

	/* load and initialize a Java VM, return a JNI interface
	* pointer in env */
	int ret = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
	assert(ret >= 0);

	delete options;

	invoke_class(env);

#if 0
	/* invoke the Main.test method using the JNI */
	jclass cls = env->FindClass("Main");
	jmethodID mid = env->GetStaticMethodID(cls, "test", "(I)V");
	env->CallStaticVoidMethod(cls, mid, 100);
#endif

	/* We are done. */
	jvm->DestroyJavaVM();
	return 0;
}
 
int main(array<System::String ^> ^args)
{
	int err = 0;
	jobject networkTable = NULL;
	
//	NetworkTable::initialize();
	Console::WriteLine(L"Create VM");
	// int err = CreateInvokeAndReleaseVM1();
	err = createJVM();

	Console::WriteLine(L"Invoke Java Magic");
	// err = invokeMagic( env );
	err = networkTableSetClientMode( env );
	err = networkTableSetIPAddress( env, "172.16.203.140" );
	networkTable = networkTableGetTable( env, "datatable" );

	err = networkTablePutNumber( env, networkTable, "bz", 7.654321);

	String^ userInput;
	do
	{
		Console::Write("Enter text:");
		userInput = Console::ReadLine();
		if (String::IsNullOrEmpty(userInput))
		{ Console::WriteLine("Thanks"); }
		else
		{ Console::WriteLine("Input [" + userInput + "]"); 
		err = networkTablePutString( env, networkTable, "samplestring", userInput);
		// other code to process user input
		}
	} while (!String::IsNullOrEmpty(userInput));

	Console::WriteLine(L"Destroy VM");
	// err = releaseJVM();

	return err;
}


namespace JBridge 
{

public ref class JniBridge
{
    // TODO: Add your methods for this class here.


public:
    void HelloWorldTest()
    {
        System::Console::WriteLine("Hello Worldl from managed C++!");
    }

    JNIEnv* create_vm(JavaVM ** jvm) 
    {
        JNIEnv *env;
        JavaVMInitArgs vm_args;

        JavaVMOption options; 
        //Path to the java source code     
        options.optionString = "-Djava.class.path=D:\\Java Src\\TestStruct"; 
        vm_args.version = JNI_VERSION_1_6; //JDK version. This indicates version 1.6
        vm_args.nOptions = 1;
        vm_args.options = &options;
        vm_args.ignoreUnrecognized = 0;

        int ret = JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
        if(ret < 0)
            printf("\nUnable to Launch JVM\n");       
        return env;
    }
    };
}