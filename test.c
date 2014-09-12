//
//  test.c
//  CFObjectClass
//
//  Created by Sam Marshall on 4/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include "test.h"

#pragma mark -
#pragma mark Private

CFObjectInit(EXRange) {
	EXRangeRef rangeref = (EXRangeRef)cf;
	EXRangeSetLocation(rangeref, 0);
	EXRangeSetLength(rangeref, 0);
}

CFObjectEqual(EXRange) {
    EXRangeRef rangeref1 = (EXRangeRef)cf1;
    EXRangeRef rangeref2 = (EXRangeRef)cf2;
	
    if (EXRangeGetLength(rangeref1) != EXRangeGetLength(rangeref2)) {
		return false;
	}
	
    if (EXRangeGetLocation(rangeref1) != EXRangeGetLocation(rangeref2)) {
		return false;
	}
	
    return true;
};

CFObjectHash(EXRange) {
    EXRangeRef rangeref = (EXRangeRef)cf;
    return (CFHashCode)(rangeref->ivars._location + rangeref->ivars._length);
};

CFObjectCopy(EXRange) {
	EXRangeRef rangeref = (EXRangeRef)cf;
	return rangeref;
};

CFObjectCopyDescription(EXRange) {
    EXRangeRef rangeref = (EXRangeRef)cf;
    return CFStringCreateWithFormat(CFGetAllocator(rangeref), formatOpts, CFSTR("[%u, %u)"), rangeref->ivars._location, rangeref->ivars._location + rangeref->ivars._length);
};

CFObjectCopyDebugDescription(EXRange) {
    EXRangeRef rangeref = (EXRangeRef)cf;
    return CFStringCreateWithFormat(CFGetAllocator(rangeref), NULL, CFSTR("<EXRange %p [%p]>{loc = %u, len = %u}"), rangeref, CFGetAllocator(rangeref), rangeref->ivars._location, rangeref->ivars._length);
};

CFObjectRelease(EXRange) {
	EXRangeRef rangeref = (EXRangeRef)cf;
	EXRangeSetLocation(rangeref, 0);
	EXRangeSetLength(rangeref, 0);
};

CFObjectInitializeClass(EXRange)

#pragma mark -
#pragma mark Public

CFObjectCreateImpForCreate(EXRange, (CFAllocatorRef allocator, uint32_t location, uint32_t length)) {
	CFObjectCreate(EXRange)
	
	EXRangeSetLength(obj, length);
	EXRangeSetLocation(obj, location);
	
    return (EXRangeRef)obj;
}

CFObjectCreateImpForGetIvar(EXRange, uint32_t, Location) {
	EXRangeRef rangeref = (EXRangeRef)cf;
	return rangeref->ivars._location;
};

CFObjectCreateImpForSetIvar(EXRange, uint32_t, Location) {
	EXRangeRef rangeref = (EXRangeRef)cf;
	memcpy((void*)&(rangeref->ivars._location), &value, sizeof(uint32_t));
};


CFObjectCreateImpForGetIvar(EXRange, uint32_t, Length) {
	EXRangeRef rangeref = (EXRangeRef)cf;
	return rangeref->ivars._length;
};

CFObjectCreateImpForSetIvar(EXRange, uint32_t, Length) {
	EXRangeRef rangeref = (EXRangeRef)cf;
	memcpy((void*)&(rangeref->ivars._length), &value, sizeof(uint32_t));
};
