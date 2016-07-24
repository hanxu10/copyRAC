//
//  EXTScope.h
//  RAC
//
//  Created by 旭旭 on 16/7/19.
//  Copyright © 2016年 yd. All rights reserved.
//

#import "metamacro.h"


//weakify(self)展开为：
//1.不在debug模式下
//try {} @catch (...) {}  metamacro_foreach_cxt1(rac_weakify_,,__weak,self)
//
//try {} @catch (...) {}  rac_weakify_(0, __weak, self)
//
//try {} @catch (...) {} __weak __typeof__(self) self_weak_ = self;
//
//2.在debug模式下
//autorelease{} __weak __typeof__(self) self_weak_ = self;
//
//
//
#define weakify(...) rac_keywordify metamacro_foreach_cxt(rac_weakify_,,__weak, __VA_ARGS__)

#define strongify(...) \
    rac_keywordify _Pragma("clang diagnostic push")\
    _Pragma("clang diagnostic ignored \"-Wshadow\"") \
    metamacro_foreach(rac_strongify_,,__VA_ARGS_)

#define unsafeify(...) rac_keywordify metamacro_foreach_cxt(rac_weakify_,,__unsafe_unretained, __VA_ARGS__)




/**
 *  实现细节
 */

typedef void (^rac_cleanupBlock_t)();

static inline void rac_executeCleanupBlock (__strong rac_cleanupBlock_t *block) {
    (*block)();
}

#define rac_weakify_(INDEX, CONTEXT, VAR) CONTEXT __typeof__(VAR) metamacro_concat(VAR, _weak_) = (VAR);

#define rac_strongify_(INDEX, VAR) __strong __typeof__(VAR) VAR = metamacro_concat(VAR, _weak_);


#if DEBUG
#define rac_keywordify autoreleasepool {}
#else
#define rac_keywordify try {} @catch (...) {}
#endif












