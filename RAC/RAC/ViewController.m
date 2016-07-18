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

void f1(){
    printf("dddd\n");
}
#define ppp(y,x) NSLog(@"haha--%@",x)

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    metamacro_foreach(ppp, ;, @"2",@"3");
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
