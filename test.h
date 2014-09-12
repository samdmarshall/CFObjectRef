//
//  test.h
//  CFObjectClass
//
//  Created by Sam Marshall on 4/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef CFObjectClass_test_h
#define CFObjectClass_test_h

#include "CFObjectRef.h"

// ========================= EXAMPLE =========================

// Example: EXRange -- a "range" object, which keeps the starting
//       location and length of the range. ("EX" as in "EXample").

// ---- API ----

CreateCFObjectWithNameAndProperties(EXRange, {
	uint32_t _location;
    uint32_t _length;
})

CFObjectCreateForClass(EXRange, (CFAllocatorRef allocator, uint32_t location, uint32_t length))

CFObjectCreateGetSetForIvar(EXRange, uint32_t, Location)
CFObjectCreateGetSetForIvar(EXRange, uint32_t, Length)

#endif
