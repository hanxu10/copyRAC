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
//try {} @catch (...) {}  metamacro_foreach_cxt1(rac_weakify,,__weak,self)
//
//try {} @catch (...) {}  rac_weakify(0, __weak, self)
//
//try {} @catch (...) {} __weak __typeof__(self) self_weak_ = self;
//
//2.在debug模式下
//autorelease{} __weak __typeof__(self) self_weak_ = self;
//
//
//
#define weakify(...) rac_keywordify metamacro_foreach_cxt(rac_weakify,,__weak, __VA_ARGS__)