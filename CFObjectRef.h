//
//  CFObjectRef.h
//  CFObjectClass
//
//  Created by Sam Marshall on 4/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef CFObjectClass_CFObjectRef_h
#define CFObjectClass_CFObjectRef_h

#include <CoreFoundation/CoreFoundation.h>
#include "CFRuntime.h"

#pragma mark -
#pragma mark Interface

#define ComposeName(prefix, name, affix) prefix##name##affix

// Creating the object "ref" name
#define __CFRef(name) ComposeName(,name,Ref)


#define CreateCFObjectWithNameAndProperties(name, ivar_struct) \
struct ComposeName(__,name,Ivars) ivar_struct; \
struct ComposeName(__,name,) { \
	CFRuntimeBase _base; \
	struct ComposeName(__,name,Ivars) ivars; \
}; \
typedef const struct ComposeName(__,name,) * __CFRef(name); \
CFTypeID ComposeName(,name,GetTypeID)(void);


#define CFObjectCreateForClass(class_name, args) __CFRef(class_name) ComposeName(,class_name,Create) args ;


#define CFObjectCreateGetterNameForIvar(class_name, ivar) ComposeName(class_name,Get,ivar)


#define CFObjectCreateSetterNameForIvar(class_name, ivar) ComposeName(class_name,Set,ivar)

/*!
 @function CreateGetSetForIvar
 @param class_name The name of the class
 @param type The type of the ivar
 @param ivar The ivar name
 */
#define CFObjectCreateGetSetForIvar(class_name, type, ivar) \
type CFObjectCreateGetterNameForIvar(class_name, ivar)(__CFRef(class_name) ref); \
void CFObjectCreateSetterNameForIvar(class_name, ivar)(__CFRef(class_name) ref, type value);


#pragma mark -
#pragma mark Implementation


#define __CFObjectBaseDeclaration(class_name, call_type, return_type, args) static return_type ComposeName(__,class_name,call_type) args

/*!
 @function CFObjectInit
 Performs additional initialization of a CF object
 @param cf A CF object
 @result A fully initialized CF object
 */
#define CFObjectInit(class_name) __CFObjectBaseDeclaration(class_name, Init, void, (CFTypeRef cf))

/*!
 @function CFObjectCopy
 @param cf A CF object to copy
 @result A copy of the passed CF object
 */
#define CFObjectCopy(class_name) __CFObjectBaseDeclaration(class_name, Copy, CFTypeRef, (CFAllocatorRef allocator, CFTypeRef cf))

/*!
 @function CFObjectRelease
 Frees memory used by a CF object
 @param cf A CF object
 */
#define CFObjectRelease(class_name) __CFObjectBaseDeclaration(class_name, Finalize, void, (CFTypeRef cf))

/*!
 @function CFObjectEqual
 Tests if two CF objects are equal
 @param cf1 A CF object
 @param cf2 A CF object
 @result Boolean value of if the two objects are equal
 */
#define CFObjectEqual(class_name) __CFObjectBaseDeclaration(class_name, Equal, Boolean, (CFTypeRef cf1, CFTypeRef cf2))

/*!
 @function CFObjectHash
 Gets the hash value associated with a CF object
 @param cf A CF Object
 @result Hash value
 */
#define CFObjectHash(class_name) __CFObjectBaseDeclaration(class_name, Hash, CFHashCode, (CFTypeRef cf))

/*!
 @function CFObjectCopyDescription
 Prints the description of an object
 @param cf A CF object
 */
#define CFObjectCopyDescription(class_name) __CFObjectBaseDeclaration(class_name, CopyFormattingDesc, CFStringRef, (CFTypeRef cf, CFDictionaryRef formatOpts))

/*!
 @function CFObjectCopyDebugDescription
 Prints the debug description of an object
 @param cf A CF object
 */
#define CFObjectCopyDebugDescription(class_name) __CFObjectBaseDeclaration(class_name, CopyDebugDesc, CFStringRef, (CFTypeRef cf))


#define CFObjectClassID(class_name) ComposeName(_k,class_name,ID)


#define CFObjectClassInitializationFunc(class_name) ComposeName(__,class_name,ClassInitialize)


#define CFObjectClassRuntimeDefinition(class_name) ComposeName(_k,class_name,Class)


#define CFObjectCreate(class_name) \
if (CFObjectClassID(class_name) == _kCFRuntimeNotATypeID) { \
	CFObjectClassInitializationFunc(class_name)();\
} \
struct ComposeName(__,class_name,) *obj = NULL; \
uint32_t extra = sizeof(struct ComposeName(__,class_name,)) - sizeof(CFRuntimeBase); \
obj = (struct ComposeName(__,class_name,) *)_CFRuntimeCreateInstance(allocator, CFObjectClassID(class_name), extra, NULL);


#define CFObjectInitializeClass(class_name) \
static CFTypeID CFObjectClassID(class_name) = _kCFRuntimeNotATypeID; \
static CFRuntimeClass CFObjectClassRuntimeDefinition(class_name) = {0}; \
void CFObjectClassInitializationFunc(class_name)(void) { \
	CFObjectClassRuntimeDefinition(class_name).version = 0; \
	CFObjectClassRuntimeDefinition(class_name).className = "##class_name##"; \
	CFObjectClassRuntimeDefinition(class_name).init = ComposeName(__,class_name,Init); \
	CFObjectClassRuntimeDefinition(class_name).copy = ComposeName(__,class_name,Copy); \
	CFObjectClassRuntimeDefinition(class_name).finalize = ComposeName(__,class_name,Finalize); \
	CFObjectClassRuntimeDefinition(class_name).equal = ComposeName(__,class_name,Equal); \
	CFObjectClassRuntimeDefinition(class_name).hash = ComposeName(__,class_name,Hash); \
	CFObjectClassRuntimeDefinition(class_name).copyFormattingDesc = ComposeName(__,class_name,CopyFormattingDesc); \
	CFObjectClassRuntimeDefinition(class_name).copyDebugDesc = ComposeName(__,class_name,CopyDebugDesc); \
	CFObjectClassID(class_name) = _CFRuntimeRegisterClass((const CFRuntimeClass * const)&CFObjectClassRuntimeDefinition(class_name)); \
} \
CFTypeID ComposeName(,class_name,GetTypeID)(void) { \
    return CFObjectClassID(class_name); \
}


#define CFObjectCreateImpForCreate(class_name, args) __CFRef(class_name) ComposeName(,class_name,Create) args


#define CFObjectCreateImpForGetIvar(class_name, type, ivar) type CFObjectCreateGetterNameForIvar(class_name, ivar)(__CFRef(class_name) cf)


#define CFObjectCreateImpForSetIvar(class_name, type, ivar) void CFObjectCreateSetterNameForIvar(class_name, ivar)(__CFRef(class_name) cf, type value)


#endif
