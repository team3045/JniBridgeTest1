// JniBridgeTest.cpp : main project file.


// plagiarized from http://stackoverflow.com/questions/9999913/calling-java-methods-from-visual-c-c-using-c-cli
// This is the main DLL file.
// see also http://www.inonit.com/cygwin/jni/invocationApi/

#include "stdafx.h"
#include <jni_md.h>
#include <jni.h>

#include <assert.h>

#pragma once

using namespace System;

JavaVM *jvm = NULL;       /* denotes a Java VM */
JNIEnv *env = NULL;       /* pointer to native method interface */

// per http://saherneklawy.wordpress.com/2009/10/31/converting-systemstring-to-char/
// convert a C++ string to a char*
void string2charPtr(String ^orig, char *&out)
{
	int length = orig->Length;
	out = new char[length+1];
	for(int i=0;i<length;i++)
		out[i] = (char) orig[i];
	out[length] = 0; // null termination - warning - does NOT work with UNICODE, I am told
}

static int createJVM( void ) {
	int errReturn = -1;
	JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */

	JavaVMOption* options = new JavaVMOption[3];
	options[0].optionString = "-Djava.class.path=c:/WindRiver/WPILib/desktop-lib/networktables-desktop.jar";
	options[1].optionString = "-verbose:jni";                   /* print JNI-related messages */
	options[2].optionString = "-verbose:class";                   /* print classloader-related messages */

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

//	public static synchronized void setClientMode();
//		Signature: ()V
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

//	public static synchronized void setIPAddress(java.lang.String);
//		Signature: (Ljava/lang/String;)V
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

//	public static synchronized edu.wpi.first.wpilibj.networktables.NetworkTable getTable(java.lang.String);
//		Signature: (Ljava/lang/String;)Ledu/wpi/first/wpilibj/networktables/NetworkTable;
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

//	public void putNumber(java.lang.String, double);
//		Signature: (Ljava/lang/String;D)V
static int networkTablePutNumber( JNIEnv* env, jobject networkTable, char* itemName, double n ) {
	int errReturn = -1;

	jclass networkTableClass;
	jmethodID putNumber;

	// Get a class reference for this object
	networkTableClass = env->GetObjectClass(networkTable);
	assert(networkTableClass != 0);
 
	putNumber = env->GetMethodID(networkTableClass, "putNumber", "(Ljava/lang/String;D)V");
	assert(putNumber != 0);

	jstring jitemName = env->NewStringUTF( itemName ); 
	jdouble doubleArgument = n;
	env->CallVoidMethod(networkTable, putNumber, jitemName, doubleArgument);

	errReturn = 0;
	return errReturn;
}

//	public void putString(java.lang.String, java.lang.String);
//		Signature: (Ljava/lang/String;Ljava/lang/String;)V
static int networkTablePutString( JNIEnv* env, jobject networkTable, char* itemName, String^ itemString ) {
	int errReturn = -1;

	jclass networkTableClass;
	jmethodID putString;

	networkTableClass = env->GetObjectClass(networkTable);
	assert(networkTableClass != 0);
 
	putString = env->GetMethodID(networkTableClass, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
	assert(putString != 0);

	jstring jitemName = env->NewStringUTF( itemName );
	char* cItemString = NULL;
	string2charPtr(itemString, cItemString);
	jstring jitemString = env->NewStringUTF( cItemString );
	delete cItemString; cItemString = NULL;
	env->CallVoidMethod(networkTable, putString, jitemName, jitemString);

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

int main(array<System::String ^> ^args)
{
	int err = 0;
	jobject networkTable = NULL;
	
	Console::WriteLine(L"Create VM");
	err = createJVM();

	Console::WriteLine(L"Invoke Java Magic");
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
	// the above is commented out because it hangs; it probably hangs because of some issues
	// with local and global references to Java objects.  But, since we're done anyway, for now we can skip this.

	return err;
}
