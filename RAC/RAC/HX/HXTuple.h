//
//  HXTuple.h
//  RAC
//
//  Created by 旭旭 on 16/7/17.
//  Copyright © 2016年 yd. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "metamacros.h"
@class HXSequence;
//创建一个新的元组，至少给1个参数，参数不能是nil
#define HXTuplePack(...) HXTuplePack_(__VA_ARGS__)


//声明新的对象变量，并把HXTuple解包到它们
//
//这个宏用在赋值操作的左边，tuple放在右边。
//在同一行不要放别东西了，并且这个宏不应该成为条件语句和循环体中的唯一一句。
//
//如果tuple中值的个数多余左边的变量的个数，多余的值被忽略。
//
//如果tuple中值的个数少于左边的变量的个数，多余的变量被初始化为nil
//
//   Examples
//
//   RACTupleUnpack(NSString *string, NSNumber *num) = [RACTuple tupleWithObjects:@"foo", @5, nil];
//   NSLog(@"string: %@", string);
//   NSLog(@"num: %@", num);
//
//   /* The above is equivalent to: */
//   RACTuple *t = [RACTuple tupleWithObjects:@"foo", @5, nil];
//   NSString *string = t[0];
//   NSNumber *num = t[1];
//   NSLog(@"string: %@", string);
//   NSLog(@"num: %@", num);
#define HXTupleUnpack(...) HXTupleUnpack_(__VA_ARGS__)



//在tuple中代表nil的哨兵对象
//永远不要自己创建一个tuple nil，使用+tupleNil就好了
@interface HXTupleNil : NSObject <NSCopying, NSCoding>
//单例
+ (HXTupleNil *)tupleNil;

@end


//一个tuple是一些对象的有序集合，可以包含nils（HXTupleNil）
@interface HXTuple : NSObject <NSCoding, NSCopying, NSFastEnumeration>

@property (nonatomic, readonly) NSUInteger count;



@property (nonatomic, readonly) id first;
@property (nonatomic, readonly) id second;
@property (nonatomic, readonly) id third;
@property (nonatomic, readonly) id fourth;
@property (nonatomic, readonly) id fifth;
@property (nonatomic, readonly) id last;

+ (instancetype)tupleWithObjectsFromArray:(NSArray *)array;

//如果convert是YES，它会把NSNull转成HXTupleNil。
+ (instancetype)tupleWithObjectsFromArray:(NSArray *)array convertNullsToNils:(BOOL)convert;


+ (instancetype)tupleWithObjects:(id)object, ... NS_REQUIRES_NIL_TERMINATION;


- (instancetype)objectAtIndex:(NSUInteger)index;

//把元组中的元素用数组返回，如果里面有HXTupleNil，转成NSNulls。
- (NSArray *)allObjects;

//obj可以是nil
- (instancetype)tupleByAddingObject:(id)obj;

@end


@interface HXTuple (HXSequenceAdditions)
@property (nonatomic, copy, readonly) HXSequence *hx_sequence;
@end

@interface HXTuple (ObjextSubscripting)
- (id)objectAtIndexedSubscript:(NSUInteger)idx;
@end



/*............................................................*/
/*............................................................*/
/*................下面的方法是用于内部调用的......................*/
#define HXTuplePack_(...) ([HXTuple tupleWithObjectsFromArray:@[ metamacro_foreach(HXTuplePack_object_or_hxtuplenil,, __VA_ARGS__) ]])

#define HXTuplePack_object_or_hxtuplenil(INDEX, ARG) (ARG) ?:HXTupleNil.tupleNil,

//   HXTupleUnpack(NSString *string, NSNumber *num) = [HXTuple tupleWithObjects:@"foo", @5, nil];
//   对其左边进行展开：
//
//   HXTupleUnpack_decl(0, NSString *string)HXTupleUnpack_decl(1, NSNumber *num)
//
//   int HXTupleUnpack_state_101 = 0;
//   HXTupleUnpack_after_101:;
//   HXTupleUnpack_assign(0, NSString *string) HXTupleUnpack_assign(1, NSNumber *num)
//   if (HXTupleUnpack_state_101 != 0) HXTupleUnpack_state_101 = 2;
//   while(HXTupleUnpack_state_101 != 2)
//      if(HXTupleUnpack_state_101 == 1) {
//          goto HXTupleUnpack_after_101;
//      } else
//          for(; HXTupleUnpack_state_101 != 1; HXTupleUnpack_state_101 = 1)
//              [RACTupleUnpackingTrampoline trampoline][@[HXTupleUnpack_value(0,NSString *string) HXTupleUnpack_value(1,NSNumber *num)]]
//
//   接着展开：
//   __strong id HXTupleUnpack_decl_name(0); __strong id HXTupleUnpack_decl_name(1);
//   int HXTupleUnpack_state_101 = 0;
//   HXTupleUnpack_after_101:;
//   __strong NSString *string = RACTupleUnpack_decl_name(0);__strong NSNumber *num = RACTupleUnpack_decl_name(1);
//   if (HXTupleUnpack_state_101 != 0) HXTupleUnpack_state_101 = 2;
//   while(HXTupleUnpack_state_101 != 2)
//      if(HXTupleUnpack_state_101 == 1) {
//          goto HXTupleUnpack_after_101;
//      } else
//          for(; HXTupleUnpack_state_101 != 1; HXTupleUnpack_state_101 = 1)
//              [RACTupleUnpackingTrampoline trampoline][@[[NSValue valueWithPointer:&RACTupleUnpack_decl_name(0)], [NSValue valueWithPointer:&RACTupleUnpack_decl_name(1)],]]
//
//  接着展开:
//   __strong id  RACTupleUnpack101_var0;
//   __strong id  RACTupleUnpack101_var1;
//   int HXTupleUnpack_state_101 = 0;
//   HXTupleUnpack_after_101:;
//   __strong NSString *string = RACTupleUnpack101_var0;
//   __strong NSNumber *num = RACTupleUnpack101_var1;
//   if (HXTupleUnpack_state_101 != 0) HXTupleUnpack_state_101 = 2;
//   while(HXTupleUnpack_state_101 != 2)
//      if(HXTupleUnpack_state_101 == 1) {
//          goto HXTupleUnpack_after_101;
//      } else
//          for(; HXTupleUnpack_state_101 != 1; HXTupleUnpack_state_101 = 1)
//              [RACTupleUnpackingTrampoline trampoline][@[[NSValue valueWithPointer:&RACTupleUnpack101_var0], [NSValue valueWithPointer:&RACTupleUnpack101_var1)],]] = [HXTuple tupleWithObjects:@"foo", @5, nil];
#define HXTupleUnpack_(...) \
    metamacro_foreach(HXTupleUnpack_decl,, __VA_ARGS__) \
    \
    int HXTupleUnpack_state = 0; \
    \
    HXTupleUnpack_after: \
        ;\
        metamacro_foreach(HXTupleUnpack_assign,, __VA_ARGS__) \
        if (HXTupleUnpack_state != 0) HXTupleUnpack_state = 2; \
        \
        while (HXTupleUnpack_state != 2) \
            if (HXTupleUnpack_state == 1) { \
                goto HXTupleUnpack_after; \
            } else \
                for(; HXTupleUnpack_state != 1; HXTupleUnpack_state = 1) \
                    [HXTupleUnpackingTrampoline trampoline][ @[ metamacro_foreach(HXTupleUnpack_value,, __VA_ARGS__) ] ]

#define HXTupleUnpack_state metamacro_concat(HXTupleUnpack_state, __LINE__)
#define HXTupleUnpack_after metamacro_concat(HXTupleUnpack_after, __LINE__)
#define HXTupleUnpack_loop metamacro_concat(HXTupleUnpack_loop, __LINE__)


#define HXTupleUnpack_decl_name(INDEX) \
    metamacro_concat(metamacro_concat(HXTupleUnpack, __LINE__), metamacro_concat(_var, INDEX))
#define HXTupleUnpack_decl(INDEX, ARG) __strong id HXTupleUnpack_decl_name(INDEX);

#define HXTupleUnpack_assign(INDEX, ARG) __strong ARG = HXTupleUnpack_decl_name(INDEX);


#define HXTupleUnpack_value(INDEX, ARG) \
    [NSValue valueWithPointer:&HXTupleUnpack_decl_name(INDEX)],





@interface HXTupleUnpackingTrampoline : NSObject

+ (instancetype)trampoline;
- (void)setObject:(HXTuple *)tuple forKeyedSubscript:(NSArray *)variables;

@end










