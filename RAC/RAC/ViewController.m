//
//  ViewController.m
//  RAC
//
//  Created by mac on 16/7/13.
//  Copyright © 2016年 yd. All rights reserved.
//

#import "ViewController.h"
#import "metamacros.h"
@interface ViewController ()

@end


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    
    metamacro_if_eq(1, 1) (NSLog(@"相当")) (NSLog(@"不相等"));
    
    NSLog(@"");
}

- (NSInteger)sss {
    NSLog(@"sadfasdf");
    return 1;
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
