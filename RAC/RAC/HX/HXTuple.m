//
//  HXTuple.m
//  RAC
//
//  Created by 旭旭 on 16/7/17.
//  Copyright © 2016年 yd. All rights reserved.
//

#import "HXTuple.h"
#import "EXTKeyPathCoding.h"
#import "HXTupleSequence.h"
@implementation HXTupleNil

+ (HXTupleNil *)tupleNil {
    static dispatch_once_t onceToken;
    static HXTupleNil *tupleNil = nil;
    dispatch_once(&onceToken, ^{
        tupleNil = [[self alloc]init];
    });
    return tupleNil;
}

#pragma mark NSCopying
- (id)copyWithZone:(NSZone *)zone {
    return self;
}
#pragma mark NSCoding
- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    return self.class.tupleNil;
}
- (void)encodeWithCoder:(NSCoder *)aCoder {
    
}
@end

@interface HXTuple ()
@property (nonatomic, strong)NSArray *backingArray;
@end

@implementation HXTuple

- (instancetype)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    self.backingArray = [NSArray array];
    return self;
}

- (NSString *)description {
    return [NSString stringWithFormat:@"<%@: %p> %@", self.class, self, self.allObjects];
}

- (BOOL)isEqual:(HXTuple *)object {
    if (object == self) {
        return YES;
    }
    if (![object isKindOfClass:self.class]) {
        return NO;
    }
    return [self.backingArray isEqual:object.backingArray];
}

- (NSUInteger)hash {
    return self.backingArray.hash;
}

#pragma mark NSFastEnumeration
- (NSUInteger)countByEnumeratingWithState:(NSFastEnumerationState *)state objects:(__unsafe_unretained id  _Nonnull *)buffer count:(NSUInteger)len {
    return [self.backingArray countByEnumeratingWithState:state objects:buffer count:len];
}

#pragma mark NSCopying
- (id)copyWithZone:(NSZone *)zone {
    //我们是不可变的
    return self;
}

#pragma mark NSCoding
- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    if (self = [self init]) {
        self.backingArray = [aDecoder decodeObjectForKey:@keypath(self.backingArray)];
    }
    return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
    if (self.backingArray) {
        [aCoder encodeObject:self.backingArray forKey:@keypath(self.backingArray)];
    }
}

#pragma mark API
+ (instancetype)tupleWithObjectsFromArray:(NSArray *)array {
    return [self tupleWithObjectsFromArray:array convertNullsToNils:NO];
}

+ (instancetype)tupleWithObjectsFromArray:(NSArray *)array convertNullsToNils:(BOOL)convert {
    HXTuple *tuple = [[self alloc]init];
    
    if (convert) {
        NSMutableArray *newArray = [NSMutableArray arrayWithCapacity:array.count];
        for (id object in array) {
            [newArray addObject:(object == NSNull.null ? HXTupleNil.tupleNil : object)];
        }
        tuple.backingArray = newArray;
    } else {
        tuple.backingArray = [array copy];
    }
    return tuple;
}

+ (instancetype)tupleWithObjects:(id)object, ... {
    HXTuple *tuple = [[self alloc]init];
    
    va_list args;
    va_start(args, object);
    
    NSUInteger count = 0;
    for (id currentObject = object; currentObject != nil; currentObject = va_arg(args, id)) {
        ++count;
    }
    
    va_end(args);
    
    if (count == 0) {
        tuple.backingArray = @[];
        return tuple;
    }
    
    NSMutableArray *objects = [[NSMutableArray alloc]initWithCapacity:count];
    
    va_start(args, object);
    for (id currentObject = object; currentObject != nil; currentObject = va_arg(args, id)) {
        [objects addObject:currentObject];
    }
    va_end(args);
    
    tuple.backingArray = objects;
    return tuple;
}


- (instancetype)objectAtIndex:(NSUInteger)index {
    if (index >= self.count) {
        return nil;
    }
    id object = self.backingArray[index];
    return (object == HXTupleNil.tupleNil ? NSNull.null : object);
}

- (NSArray *)allObjects {
    NSMutableArray *newArray = [NSMutableArray arrayWithCapacity:self.backingArray.count];
    for (id object in self.backingArray) {
        [newArray addObject:(object == HXTupleNil.tupleNil ? NSNull.null : object)];
    }
    
    return newArray;
}


- (instancetype)tupleByAddingObject:(id)obj {
    NSArray *newArray = [self.backingArray arrayByAddingObject:obj ?: HXTupleNil.tupleNil];
    return [self.class tupleWithObjectsFromArray:newArray];
}

- (NSUInteger)count {
    return self.backingArray.count;
}

- (id)first {
    return self[0];
}
- (id)second {
    return self[1];
}

- (id)third {
    return self[2];
}

- (id)fourth {
    return self[3];
}

- (id)fifth {
    return self[4];
}

- (id)last {
    return self[self.count - 1];
}


@end

@implementation HXTuple (HXSequenceAdditions)

- (HXSequence *)hx_sequence {
    return [HXTupleSequence se]
}

@end


@implementation HXTuple (ObjectSubscripting)

- (id)objectAtIndexedSubscript:(NSUInteger)idx {
    return [self objectAtIndex:idx];
}

@end

@implementation HXTupleUnpackingTrampoline

+ (instancetype)trampoline {
    static dispatch_once_t onceToken;
    static id trampoline = nil;
    dispatch_once(&onceToken, ^{
        trampoline = [[self alloc]init];
    });
    return trampoline;
}

- (void)setObject:(HXTuple *)tuple forKeyedSubscript:(NSArray *)variables {
    NSCParameterAssert(variables != nil);
    
    [variables enumerateObjectsUsingBlock:^(NSValue *value, NSUInteger idx, BOOL *stop) {
        __strong id *ptr = (__strong id *)value.pointerValue;
        *ptr = tuple[idx];
    }];
}
@end





