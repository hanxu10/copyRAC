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

