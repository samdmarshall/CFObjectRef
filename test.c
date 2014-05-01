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

CFObjectEqual(EXRange, {
    EXRangeRef rangeref1 = (EXRangeRef)cf1;
    EXRangeRef rangeref2 = (EXRangeRef)cf2;
    if (rangeref1->ivars._location != rangeref2->ivars._location) return false;
    if (rangeref1->ivars._length != rangeref2->ivars._length) return false;
    return true;
})

CFObjectHash(EXRange, {
    EXRangeRef rangeref = (EXRangeRef)cf;
    return (CFHashCode)(rangeref->ivars._location + rangeref->ivars._length);
})


CFObjectCopyDescription(EXRange, {
    EXRangeRef rangeref = (EXRangeRef)cf;
    return CFStringCreateWithFormat(CFGetAllocator(rangeref), formatOpts, CFSTR("[%u, %u)"), rangeref->ivars._location, rangeref->ivars._location + rangeref->ivars._length);
})

CFObjectCopyDebugDescription(EXRange, {
    EXRangeRef rangeref = (EXRangeRef)cf;
    return CFStringCreateWithFormat(CFGetAllocator(rangeref), NULL, CFSTR("<EXRange %p [%p]>{loc = %u, len = %u}"), rangeref, CFGetAllocator(rangeref), rangeref->ivars._location, rangeref->ivars._length);
})

CFObjectRelease(EXRange, {
	EXRangeRef rangeref = (EXRangeRef)cf;
    // nothing to finalize
})

CFObjectInitializeClass(EXRange)

#pragma mark -
#pragma mark Public

CreateImpForCreate(EXRange, (CFAllocatorRef allocator, uint32_t location, uint32_t length), {
    struct __EXRange *newrange;
    uint32_t extra = sizeof(struct __EXRange) - sizeof(CFRuntimeBase);
    newrange = (struct __EXRange *)_CFRuntimeCreateInstance(allocator, _kEXRangeID, extra, NULL);
    if (NULL == newrange) {
		return NULL;
    }
    newrange->ivars._location = location;
    newrange->ivars._length = length;
    return (EXRangeRef)newrange;
})

CreateImpForGetIvar(EXRange, uint32_t, Location, {
	return ref->ivars._location;
})

CreateImpForSetIvar(EXRange, uint32_t, Location, {
	memcpy(&(ref->ivars._location), &value, sizeof(uint32_t));
})


CreateImpForGetIvar(EXRange, uint32_t, Length, {
	return ref->ivars._length;
})

CreateImpForSetIvar(EXRange, uint32_t, Length, {
	memcpy(&(ref->ivars._length), &value, sizeof(uint32_t));
})
