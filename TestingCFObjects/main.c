//
//  main.c
//  TestingCFObjects
//
//  Created by Sam Marshall on 9/12/14.
//
//

#include <CoreFoundation/CoreFoundation.h>
#include "test.h"

int main(int argc, const char * argv[]) {
	EXRangeRef test = EXRangeCreate(kCFAllocatorDefault, 0, 5);
	EXRangeGetLocation(test);
	CFShow(test);
    return 0;
}

