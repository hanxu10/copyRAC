//
//  TableViewCell.h
//  textViewInCell
//
//  Created by 旭旭 on 16/8/25.
//  Copyright © 2016年 旭旭. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TableViewCell : UITableViewCell<UITextViewDelegate>
@property (weak, nonatomic) IBOutlet UITextView *uptv;
@property (weak, nonatomic) IBOutlet UITextView *righttv;
@property (nonatomic, copy) void (^upDidChangedText)(UITableViewCell *cell,NSString *text);
@property (nonatomic, copy) void (^rightDidChangedText)(UITableViewCell *cell,NSString *text);
@end
