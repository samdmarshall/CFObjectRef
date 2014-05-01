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

#define __CFRef(name) name##Ref

#define CreateCFObjectWithNameAndProperties(name, ivar_struct) \
struct __##name##Ivars ivar_struct; \
struct __##name { \
	CFRuntimeBase _base; \
	struct __##name##Ivars ivars; \
}; \
typedef const struct __##name * __CFRef(name); \
CFTypeID name##GetTypeID(void);

#define CreateForClass(class_name, args) __CFRef(class_name) class_name##Create args ;

#define CreateGetSetForIvar(class_name, type, ivar) \
type class_name##Get##ivar(__CFRef(class_name) ref); \
void class_name##Set##ivar(__CFRef(class_name) ref, type value);

#pragma mark -
#pragma mark Implementation

#define __CFObjectBaseDeclaration(class_name, call_type, return_type, args, imp) static return_type __##class_name##call_type args imp

#define CFObjectInit(class_name, init_imp) __CFObjectBaseDeclaration(class_name, Init, CFTypeRef, (CFTypeRef cf1, CFTypeRef cf2), init_imp)
#define CFObjectCopy(class_name, copy_imp) __CFObjectBaseDeclaration(class_name, Copy, CFTypeRef, (CFTypeRef cf), copy_imp)
#define CFObjectRelease(class_name, finalize_imp) __CFObjectBaseDeclaration(class_name, Finalize, void, (CFTypeRef cf), finalize_imp)
#define CFObjectEqual(class_name, equal_imp) __CFObjectBaseDeclaration(class_name, Equal, Boolean, (CFTypeRef cf1, CFTypeRef cf2), equal_imp)
#define CFObjectHash(class_name, hash_imp) __CFObjectBaseDeclaration(class_name, Hash, CFHashCode, (CFTypeRef cf), hash_imp)
#define CFObjectCopyDescription(class_name, desc_imp) __CFObjectBaseDeclaration(class_name, CopyFormattingDesc, CFStringRef, (CFTypeRef cf, CFDictionaryRef formatOpts), desc_imp)
#define CFObjectCopyDebugDescription(class_name, ddesc_imp) __CFObjectBaseDeclaration(class_name, CopyDebugDesc, CFStringRef, (CFTypeRef cf), ddesc_imp)

#define CFObjectInitializeClass(class_name) \
static CFTypeID _k##class_name##ID = _kCFRuntimeNotATypeID; \
static CFRuntimeClass _k##class_name##Class = {0}; \
void __##class_name##ClassInitialize(void) { \
	_k##class_name##Class.version = 0; \
	_k##class_name##Class.className = "##class_name##"; \
	_k##class_name##Class.init = NULL; \
	_k##class_name##Class.copy = NULL; \
	_k##class_name##Class.finalize = __##class_name##Finalize; \
	_k##class_name##Class.equal = __##class_name##Equal; \
	_k##class_name##Class.hash = __##class_name##Hash; \
	_k##class_name##Class.copyFormattingDesc = __##class_name##CopyFormattingDesc; \
	_k##class_name##Class.copyDebugDesc = __##class_name##CopyDebugDesc; \
	_k##class_name##ID = _CFRuntimeRegisterClass((const CFRuntimeClass * const)&_k##class_name##Class); \
} \
CFTypeID class_name##GetTypeID(void) { \
    return _k##class_name##ID; \
}

#define CreateImpForCreate(class_name, args, create_imp) __CFRef(class_name) class_name##Create args create_imp

#define CreateImpForGetIvar(class_name, type, ivar, get_imp) type class_name##Get##ivar(__CFRef(class_name) ref) get_imp
#define CreateImpForSetIvar(class_name, type, ivar, set_imp) void class_name##Set##ivar(__CFRef(class_name) ref, type value) set_imp

#endif
