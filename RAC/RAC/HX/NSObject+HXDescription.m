//
//  NSObject+NSObject_HXDescription.m
//  RAC
//
//  Created by 旭旭 on 16/7/23.
//  Copyright © 2016年 yd. All rights reserved.
//

#import "NSObject+HXDescription.h"
#import "HXTuple.h"
@implementation NSValue (HXDescription)

- (NSString *)hx_description {
    return self.description;
}

@end


@implementation NSString (HXDescription)

- (NSString *)hx_description {
    return self.description;
}

@end

@implementation HXTuple (HXDescription)
- (NSString *)hx_description {
    if (getenv("RAC_DEBUG_SIGNAL_NAMES") != NULL) {
        return self.allObjects.description;
    } else {
        return @"description skipped";
    }
}

@end


NSString *HXDescription(id object) {
    if (getenv("RAC_DEBUG_SIGNAL_NAMES") != NULL) {
        if ([object respondsToSelector:@selector(hx_description)]) {
            return [object hx_description];
        } else {
            return [[NSString alloc]initWithFormat:@"<%@: %p>", [object class], object];
        }
    } else {
        return @"(description skipped)";
    }
}




