//
//  HXStream.h
//  RAC
//
//  Created by 旭旭 on 16/7/13.
//  Copyright © 2016年 yd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HXStream : NSObject

//返回一个stream，这个stream由‘value’和这个receiver的值组成。
- (instancetype)startWith:(id)value;


//跳过了开始的skipCount个值后，返回这个receiver。如果skipCount大于stream中值的个数时，就会返回一个empty stream。
//skip是跳过前几个
- (instancetype)skip:(NSUInteger)skipCount;


//take是取前几个
- (instancetype)take:(NSUInteger)count;


//跟之前的实例方法一样
//zip是用每个stream的第1个，第2个...第N个组成元组
//（注意与combineLatest进行比较，combineLatest是用每个signal最近的next组成，最开始的时候每个signal都得发出一个next才能组成一个元组，之后只要有一个next就会产生一个元组）
+ (instancetype)zip:(id<NSFastEnumeration>)streams;


// Example:
//
//   [RACStream zip:@[ stringSignal, intSignal ] reduce:^(NSString *string, NSNumber *number) {
//       return [NSString stringWithFormat:@"%@: %@", string, number];
//   }];
+ (instancetype)zip:(id<NSFastEnumeration>)streams reduce:(id (^)())reduceBlock;


//按照顺序，先订阅第一个stream，当第一个complete时，再订阅第2个stream........
+ (instancetype)concat:(id<NSFastEnumeration>)streams;


/// Combines values in the receiver from left to right using the given block.
///
/// The algorithm proceeds as follows:
///
///  1. `startingValue` is passed into the block as the `running` value, and the
///  first element of the receiver is passed into the block as the `next` value.
///  2. The result of the invocation is added to the returned stream.
///  3. The result of the invocation (`running`) and the next element of the
///  receiver (`next`) is passed into `block`.
///  4. Steps 2 and 3 are repeated until all values have been processed.
///
/// startingValue - The value to be combined with the first element of the
///                 receiver. This value may be `nil`.
/// reduceBlock   - The block that describes how to combine values of the
///                 receiver. If the receiver is empty, this block will never be
///                 invoked. Cannot be nil.
/// 用startingValue与第一个值进行处理，得到新的第一个值，然后把新的第一个值与第二个值进行处理得到新的第二个值......
/// Examples
///
///      RACSequence *numbers = @[ @1, @2, @3, @4 ].rac_sequence;
///
///      // Contains 1, 3, 6, 10
///      RACSequence *sums = [numbers scanWithStart:@0 reduce:^(NSNumber *sum, NSNumber *next) {
///          return @(sum.integerValue + next.integerValue);
///      }];

- (instancetype)scanWithStart:(id)startingValue reduce:(id(^)(id running, id next))reduceBlock;


//跟上面的没啥区别，就是在block中能拿到当前是第几个值
- (instancetype)scanWithStart:(id)startingValue reduceWithIndex:(id (^)(id running, id next ,NSUInteger index))reduceBlock;


/// Examples
///
///      RACSequence *numbers = @[ @1, @2, @3, @4 ].rac_sequence;
///
///      // Contains 1, 3, 5, 7
///      RACSequence *sums = [numbers combinePreviousWithStart:@0 reduce:^(NSNumber *previous, NSNumber *next) {
///          return @(previous.integerValue + next.integerValue);
///      }];
/// 相当于0和1进行处理，1和2进行处理，2和3进行处理。不像上面的那个函数会拿0和1处理的结果用于下次处理。
- (instancetype)combinePreviousWithStart:(id)start reduce:(id(^)(id previous, id current))reduceBlock;


//取值，直到这个block返回YES。（当这个block为YES的时候就不取值了）
//
- (instancetype)takeUntilBlock:(BOOL (^) (id x))predicate;


//取值，直到这个block返回NO。（当这个block为YES的时候取值）。
- (instancetype)takeWhileBlock:(BOOL (^) (id x))predicate;


//直到这个block返回YES才开始取值
- (instancetype)skipUntilBlock:(BOOL (^) (id x))predicate;


//直到这个block返回NO才开始取值。
- (instancetype)skipWhileBlock:(BOOL (^) (id x))predicate;


//返回一个stream，这个stream中值，后一个与前一个值调用isEqual方法，返回的是NO。
- (instancetype)distinctUntilChanged;


@end
