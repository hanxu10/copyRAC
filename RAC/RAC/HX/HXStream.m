//
//  HXStream.m
//  RAC
//
//  Created by 旭旭 on 16/7/13.
//  Copyright © 2016年 yd. All rights reserved.
//

#import "HXStream.h"
#import "NSObject+HXDescription.h"
#import "HXBlockTrampoline.h"
#import "HXTuple.h"
@implementation HXStream

#pragma mark - 生命周期
- (instancetype)init {
    self = [super init];
    if (self) {
        self.name = @"";
    }
    return self;
}

#pragma mark - 抽象方法
+ (instancetype)empty {
    return nil;
}

- (instancetype)bind:(HXStreamBindBlock (^)(void))block {
    return nil;
}

+ (instancetype)return:(id)value{
    return nil;
}

- (instancetype)concat:(HXStream *)stream {
    return nil;
}

- (instancetype)zipWith:(HXStream *)stream {
    return nil;
}

#pragma mark 名字相关
- (instancetype)setNameWithFormat:(NSString *)format, ... {
    if (getenv("RAC_DEBUG_SIGNAL_NAMES") == NULL) {
        return self;
    }
    
    NSCParameterAssert(format != nil);
    
    va_list args;
    va_start(args, format);
    
    NSString *str = [[NSString alloc]initWithFormat:format arguments:args];
    va_end(args);
    
    self.name = str;
    return self;
}

@end


@implementation HXStream (Operations)

- (instancetype)flattenMap:(HXStream *(^)(id))block {
    Class class = self.class;
    
    return [[self bind:^HXStreamBindBlock(){
        return ^HXStream *(id value, BOOL *stop){
            id stream = block(value) ? : [class empty];
            NSCAssert([stream isKindOfClass:HXStream.class], @"Value returned form -flattenMap: is not a stream: %@",stream);
            
            return stream;
        };
    }] setNameWithFormat:@"[%@] -flattenMap:", self.name];
}

- (instancetype)flatten {
//    明确变量即使不使用，也不会产生 warning。
    __weak HXStream *stream __attribute__((unused)) = self;
    return [[self flattenMap:^HXStream *(id value) {
        return value;
    }] setNameWithFormat:@"[%@] -flatten", self.name];
}

- (instancetype)map:(id (^)(id))block {
    NSCParameterAssert(block != nil);
    
    Class class = self.class;
    
    return [[self flattenMap:^HXStream *(id value) {
        return [class return:block(value)];
    }]setNameWithFormat:@"[%@] -map", self.name];
}

- (instancetype)mapReplace:(id)object {
    return [[self map:^id(id value) {
        return object;
    }] setNameWithFormat:@"[%@] -mapReplace: %@", self.name, HXDescription(object)];
}

- (instancetype)scanWithStart:(id)startingValue reduceWithIndex:(id (^)(id, id, NSUInteger))reduceBlock {
    NSCParameterAssert(reduceBlock != nil);
    
    Class class = self.class;
    
    return [self bind:^HXStreamBindBlock{
        __block id running = startingValue;
        __block NSUInteger index = 0;
        
        return ^(id value, BOOL *stop) {
            running = reduceBlock(running, value, index++);
            return [class return:running];
        };
    }];
}

- (instancetype)scanWithStart:(id)startingValue reduce:(id (^)(id, id))reduceBlock {
    return [[self scanWithStart:startingValue reduceWithIndex:^id(id running, id next, NSUInteger index) {
        return reduceBlock(running, next);
    }]setNameWithFormat:@"[%@] -scanWithStart: %@ reduce:", self.name, HXDescription(startingValue)];
}



- (instancetype)combinePreviousWithStart:(id)start reduce:(id (^)(id, id))reduceBlock {
    NSCParameterAssert(reduceBlock != NULL);
    return [[[self scanWithStart:HXTuplePack(start) reduce:^id(HXTuple *previousTuple, id next) {
        id value = reduceBlock(previousTuple[0], next);
        return HXTuplePack(next, value);
    }] map:^id(HXTuple *tuple) {
        return tuple[1];
    }]setNameWithFormat:@"[%@] -combinePreviousWithStart: %@ reduce:", self.name, HXDescription(start)];
}


- (instancetype)filter:(BOOL (^)(id))block {
    NSCParameterAssert(block != nil);
    
    Class class = self.class;
    
    return [[self flattenMap:^HXStream *(id value) {
        if (block(value)) {
            return [class return:value];
        }else {
            return class.empty;
        }
    }] setNameWithFormat:@"[%@] -filter:", self.name];
}

- (instancetype)ignore:(id)value {
    return [[self filter:^BOOL(id innerValue) {
        return innerValue != value && ![innerValue isEqual:value];
    }] setNameWithFormat:@"[%@] -ignore: %@", self.name, HXDescription(value)];
}

- (instancetype)reduceEach:(id (^)())reduceBlock {
    
}




@end






















