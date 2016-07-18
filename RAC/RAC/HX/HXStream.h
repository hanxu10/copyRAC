//
//  HXStream.h
//  RAC
//
//  Created by 旭旭 on 16/7/13.
//  Copyright © 2016年 yd. All rights reserved.
//

#import <Foundation/Foundation.h>

@class HXStream;

//这个类代表一个monad，monad是一个类型类（一个协议），实现了monad的类可以进行某种操作，这种操作叫做bind。
//bind操作场景：一个值装在Stream中，给一个函数（这个函数的参数是一个值，返回的是一个Stream），用这个函数去操作Stream中的值，得到一个新的Stream。
//在oc中用block来代表上面说的函数
//即：
typedef HXStream * (^HXStreamBindBlock)(id value, BOOL *stop);//这个block接受来自stream的value，返回一个新的stream。 设置'stop'为YES将在返回value后，终止bind。返回nil，将立即终止。
//如何把一个值扔到stream里呢，会在下面提供一个return:方法。

@interface HXStream : NSObject

//返回一个空的stream
+ (instancetype)empty;


//把value扔到stream monad中。
//
//返回一个装着给定value的stream。
+ (instancetype)return:(id)value;


//延时给value绑定一个block
//只在这种情况下使用：你需要早点终止这个bind，或者掩盖某些状态。 对于别的场景，-flattenMap更适合。
//
//block -这个‘block’的返回值还是一个block：HXStreamBindBlock.每次这个被绑定的stream被重新计算的时候，这个‘block’就被调用。这个‘block’不能是nil，也不能返回nil。
//
//返回一个新的streamnew，如果执行‘block’得到了一个streamA，订阅streamnew就会发送streamA的value。
- (instancetype)bind:(HXStreamBindBlock (^)(void))block;


//把‘stream’中的值拼接到方法接收者上。
//
//stream -必须和接收者是相同的类的实例，并且不能是nil。
//
//相当于先订阅了方法接收者的value，当方法接收者complete时，再订阅‘stream’。
//
//返回一个新的stream，代表方法接收者+‘stream’。
- (instancetype)concat:(HXStream *)stream;


//把receiver的值和给定的stream的值压缩成一个HXTuples。
//
//每个stream的第一个值被压缩成一个Tuples，然后第二个值被压缩,直到有一个stream没有值了。假设streamA有两个值，streamB只有一个值，那么只会产生一个Tuples。
//
//
- (instancetype)zipWith:(HXStream *)stream;

@end







//这个扩展支持命名stream来debug
//子类不需要重写这里的方法。
@interface HXStream ()

//stream的name，目的是为了dubug。
@property (copy) NSString *name;


//NS_FORMAT_FUNCTION(第一个参数是format的位置，第二个参数是可变参数开始的位置)
//设置receiver的name，除了给RAC_DEBUG_SIGNAL_NAMES环境变量设置值，不会做别的事情。
//
//返回receiver，以便进行链式操作。
- (instancetype)setNameWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1, 2);


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






@interface HXStream (Operations)

//把‘block’映射到receiver上，并且平坦化结果。
//
//block - 这个block接受receiver中的值，返回一个新的stream实例。如果返回nil，等同于返回empty stream。
//
//返回值是一个新的stream,对这个新的返回值stream进行订阅，就相当于对block中的返回值stream进行订阅。
- (instancetype)flattenMap:(HXStream *(^)(id value))block;


//使stream of streams平坦化
//
//返回一个stream，这个stream由从receiver中获取的combined streams组成。
//本来selfstream发送的是一个个stream，直接订阅selfstream拿到的值是也是stream类型的；如果对selfstream进行flatten处理，再订阅，则取到的是一个个stream发出的值。
- (instancetype)flatten;


//用block去映射receiver中的值。
//
//返回一个新的stream，这个stream中的值是block返回的值。
- (instancetype)map:(id(^)(id value))block;


//用‘object’替换receiver中的每一个值
//
//返回一个新的stream
- (instancetype)mapReplace:(id)object;


//过滤掉receiver中不符合条件的值。
//
//返回一个新的stream，这个stream的值都是通过测试的。
- (instancetype)filter:(BOOL (^)(id value))block;


//过滤掉receiver中等于给定value的值
//
//value - value可以是nil，这样的话就过滤掉nil
//
- (instancetype)ignore:(id)value;


//
//
//
- (instancetype)reduceEach:(id(^)())reduceBlock;

@end

