//
//  EXTKeyPathCoding.h
//  RAC
//
//  Created by 旭旭 on 16/7/24.
//  Copyright © 2016年 yd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "metamacros.h"


//如果传一个参数，如NSString *UTF8StringPath = @keypath(str.lowercaseString.UTF8String);
//会从第一个.号开始截取，故UTF8StringPath = @"lowercaseString.UTF8String";

//如果传入两个参数，只留下第2个参数
//NSString *lowercaseStringPath = @keypath(NSString.new, lowercaseString);
// => @"lowercaseString"

#define keypath(...) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Warc-repeated-use-of-weak\"") \
    metamacro_if_eq(1, metamacro_argcount(__VA_ARGS__)) (keypath1(__VA_ARGS__)) (keypath2(__VA_ARGS__)) \
    _Pragma("clang diagnostic pop") \


//strchr(string, tofind) strchr用来找到字符串中第一次出现某个字母的位置，把这个位置指针返回。
#define keypath1(PATH) (((void)(NO && ((void)PATH, NO)), strchr(# PATH, '.') + 1))

#define keypath2(OBJ, PATH) (((void)(NO && ((void)OBJ.PATH, NO)), # PATH))








#define collectionKeypath(...) \
metamacro_if_eq(3, metamacro_argcount(__VA_ARGS__))(collectionKeypath3(__VA_ARGS__))(collectionKeypath4(__VA_ARGS__))

#define collectionKeypath3(PATH, COLLECTION_OBJECT, COLLECTION_PATH) ([[NSString stringWithFormat:@"%s.%s",keypath(PATH), keypath(COLLECTION_OBJECT, COLLECTION_PATH)] UTF8String])

#define collectionKeypath4(OBJ, PATH, COLLECTION_OBJECT, COLLECTION_PATH) ([[NSString stringWithFormat:@"%s.%s",keypath(OBJ, PATH), keypath(COLLECTION_OBJECT, COLLECTION_PATH)] UTF8String])
