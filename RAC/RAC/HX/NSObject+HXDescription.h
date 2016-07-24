//
//  NSObject+NSObject_HXDescription.h
//  RAC
//
//  Created by 旭旭 on 16/7/23.
//  Copyright © 2016年 yd. All rights reserved.
//

#import <Foundation/Foundation.h>

//对象的简单描述，不调用-description方法，这在大多数情况下更快。

//只是为了debug，只有当RAC_DEBUG_SIGNAL_NAMES环境变量设置了，才会返回一个constant string
NSString *HXDescription(id object);