//
//  TableViewCell.m
//  textViewInCell
//
//  Created by 旭旭 on 16/8/25.
//  Copyright © 2016年 旭旭. All rights reserved.
//

#import "TableViewCell.h"

@implementation TableViewCell

- (void)awakeFromNib {
    [super awakeFromNib];
    self.uptv.delegate = self;
    self.righttv.delegate = self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}


- (void)textViewDidChange:(UITextView *)textView;
{
    if (self.uptv == textView) {
        if (self.upDidChangedText) {
            self.upDidChangedText(self,textView.text);
        }
    } else if (self.righttv == textView) {
        if (self.rightDidChangedText) {
            self.rightDidChangedText(self,textView.text);
        }
    }

    [self.tableView beginUpdates];
    [self.tableView endUpdates];
}

- (UITableView *)tableView {
    UIView *tableView = self.superview;
    
    while (![tableView isKindOfClass:[UITableView class]] && tableView) {
        tableView = tableView.superview;
    }
    
    return (UITableView *)tableView;
}

@end
