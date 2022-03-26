import numpy as np
import matplotlib.pyplot as plt
import os
from collections import OrderedDict
import logging
import collections

import write_to_BIN
import util
import data
import config
import time
import copy


logger = logging.getLogger()
logger.setLevel(logging.INFO)
logging.basicConfig(filename='logs.txt', filemode='a+', datefmt="%Y-%m-%d %H:%M:%S",
                    format="%(asctime)s--%(message)s", level=logging.INFO)

target_folder = config.target_folder
benchmark_folder = config.benchmark_folder

plt.style.use("seaborn")

elements_counter = 0


def update_parameters(row_length, element_co, element_ro, rid, cid, color_map, target_color, column_map):
    # 元素数减1
    global elements_counter
    elements_counter -= 1

    # 更新行列长度 已经行列主序映射
    row_length[rid] -= 1
    column_map[cid] -= 1
    element_co[cid].remove(rid)
    if len(element_co[cid]) == 0:
        element_co.pop(cid)
    element_ro[rid].remove(cid)

    # 更新解决方案
    color_map[target_color][0] -= 1
    color_map[target_color].append([cid, rid])


def pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order, elements_rid_order,
                                      trids, reuse_cid, color_map, target_color, column_length):
    max_row_length = max(trow_length)
    max_index = trow_length.index(max_row_length)
    update_parameters(row_length, elements_cid_order, elements_rid_order, trids[max_index], reuse_cid,
                      color_map, target_color, column_length)
    # 移除已经用过的元素
    trow_length.remove(max_row_length)
    trids.remove(trids[max_index])


def pop_one_element_block_and_update(elements_rid_order, row_length, capacity, elements_cid_order,
                                     one_element_block, color_map, target_color, column_length):
    target_cid, target_rid = util.find_component(elements_rid_order, row_length,
                                                 capacity, elements_cid_order, config.find_one, [])
    # 从 1元素块 统计里移除
    one_element_block.remove(target_cid)
    update_parameters(row_length, elements_cid_order, elements_rid_order,
                      target_rid, target_cid, color_map, target_color, column_length)
    capacity[target_cid] = config.finish


def pop_two_element_block_and_update(elements_rid_order, row_length, capacity,
                                     elements_cid_order, two_element_block, color_map, target_color, column_length):
    target_cid, target_rids = \
        util.find_component(elements_rid_order, row_length, capacity,
                            elements_cid_order, config.two_element, two_element_block)
    two_element_block.remove(target_cid)
    for rid in target_rids:
        update_parameters(row_length, elements_cid_order, elements_rid_order,
                          rid, target_cid, color_map, target_color, column_length)
    # 只有 2元素块 才置零， 别的情况不改变capacity
    if column_length[target_cid] == 0:
        capacity[target_cid] = config.finish


def pop_three_elements_wr(elements_rid_order, row_length, capacity, elements_cid_order, opcode,
                          color_map, target_color, column_length):
    target_cid, target_rids = util.find_component(elements_rid_order, row_length,
                                                  capacity, elements_cid_order, opcode, [])
    for rid in target_rids:
        update_parameters(row_length, elements_cid_order, elements_rid_order,
                          rid, target_cid, color_map, target_color, column_length)
    # 如果该列已被消耗完 置为完成
    if column_length[target_cid] == 0:
        capacity[target_cid] = config.finish
    else:
        capacity[target_cid] -= 1


def pop_one_element_wr(elements_rid_order, row_length, capacity, elements_cid_order,
                       color_map, target_color, column_length):
    target_cid, target_rid = util.find_component(elements_rid_order, row_length,
                                                 capacity, elements_cid_order, config.find_one, [])
    update_parameters(row_length, elements_cid_order, elements_rid_order,
                      target_rid, target_cid, color_map, target_color, column_length)
    capacity[target_cid] = config.finish


def pad_one_element(color_map, target_color):
    color_map[target_color][0] -= 1
    color_map[target_color].append([color_map[target_color][-1][0], config.pad_rid])


def mapping_algorithm(origin_rid: list, origin_cid: list, origin_val: list,
                      batch_index: int, output_file: str):

    # 列主序 行主序存储 方便取对应的index
    elements_cid_order = OrderedDict()
    elements_rid_order = OrderedDict()

    # 行列长度统计
    column_length = OrderedDict()
    row_length = OrderedDict()

    # 元素数量统计
    global elements_counter
    elements_counter = 0

    # 定义的容量统计
    capacity = collections.OrderedDict()

    # 解决方案存储列表
    color_map = collections.OrderedDict()

    for i in range(len(origin_rid)):
        if batch_index * config.rows <= origin_rid[i] < (batch_index + 1) * config.rows:
            # 更新统计信息
            # 列主序的格式为 cid : list(rids) 行主序的为 rid: list(cids)
            # 长度统计格式均为 cid/rid : 次数
            util.map_dict_add(elements_cid_order, {origin_cid[i]: {origin_rid[i]: origin_val[i]}})
            util.map_dict_add(elements_rid_order, {origin_rid[i]: {origin_cid[i]: 0}})
            util.counter_dict_add(column_length, origin_cid[i])
            util.counter_dict_add(row_length, origin_rid[i])
            elements_counter += 1

    original_elements_counter = elements_counter

    # 假定 1 --> -1; 2 --> 0 ; 3 --> 1; 4 --> -3; -2 --> finished
    for cid in column_length.keys():
        # 根据算法计算当前列的capacity
        capacity[cid] = util.get_capacity(column_length[cid])

    curr_capacity = util.sum_capacity(capacity)
    print("Total Capacity {} ".format(curr_capacity))

    # 组号
    target_color = 0

    # 如果需要考虑组间复用
    if curr_capacity < 0:
        # 找可复用的列
        reuseable_blocks = util.find_reuseable_columns(column_length)
        one_element_block = util.have_oneelement_block(column_length)
        two_element_block = util.have_twoelement_block(column_length)
        
        if len(reuseable_blocks) == 0:
            # 没有可复用的列 直接跳过
            pass
        elif len(reuseable_blocks) == 1:
            # 只有一列可以复用， 第一个周期的格式应该是 reuse_cid, reuse_cid, single_element, pad
            reuse_cid = reuseable_blocks[0]
            # 统计该列元素所在行的长度与行号, 均为OrderedDict, 不会乱序
            trow_length = [row_length[rid] for rid in elements_cid_order[reuse_cid]]
            trids = [rid for rid in elements_cid_order[reuse_cid]]

            color_map[target_color] = [config.elements_in_block]
            # 取该列中所在行最长的两个元素
            for _ in range(2):
                pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order, elements_rid_order,
                                                  trids, reuse_cid, color_map, target_color, column_length)

            # 返回一个 1元素块
            # 由于 capacity < 0 且 只有一个可复用列, 必存在2个以上的 1元素块
            pop_one_element_block_and_update(elements_rid_order, row_length, capacity, elements_cid_order,
                                             one_element_block, color_map, target_color, column_length)

            # pad 一个空元素
            pad_one_element(color_map, target_color)

            # 组号加1
            target_color += 1

            # 复用剩余的元素， 格式均为 reuse_cid, index1, index2, pad
            while column_length[reuse_cid] > 0:
                color_map[target_color] = [config.elements_in_block]
                # 如果有2个 1元素块 则 取2个 1元素块
                if len(one_element_block) > 1:
                    # 在复用列取一个元素
                    pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order, elements_rid_order,
                                                      trids, reuse_cid, color_map, target_color, column_length)
                    for _ in range(2):
                        pop_one_element_block_and_update(elements_rid_order, row_length, capacity, elements_cid_order,
                                                         one_element_block, color_map, target_color, column_length)
                    # pad 1个元素
                    pad_one_element(color_map, target_color)
                    # 组号加1
                    target_color += 1
                # 如果仅有 1个1元素块 以及 有2元素块
                elif len(one_element_block) == 1 and len(two_element_block) > 0:
                    # 在复用列取一个元素
                    pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order, elements_rid_order,
                                                      trids, reuse_cid, color_map, target_color, column_length)
                    # 依硬件实现， 先放2元素块
                    pop_two_element_block_and_update(elements_rid_order, row_length, capacity, elements_cid_order,
                                                     two_element_block, color_map, target_color, column_length)
                    # 取1元素块
                    pop_one_element_block_and_update(elements_rid_order, row_length, capacity, elements_cid_order,
                                                     one_element_block, color_map, target_color, column_length)
                    target_color += 1
                    # 由于没有1元素块，直接退出
                    break
                else:
                    # 没有1元素块 直接退出
                    break
            # 更新复用列的capacity
            if column_length[reuse_cid] == 0:
                capacity[reuse_cid] = config.finish
            else:
                capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
        else:
            # 如果多于2个可复用列
            reuse_cid = reuseable_blocks[0]
            trow_length = [row_length[rid] for rid in elements_cid_order[reuse_cid]]
            trids = [rid for rid in elements_cid_order[reuse_cid]]
            reuse_cid1 = reuseable_blocks[1]
            trow_length1 = [row_length[rid] for rid in elements_cid_order[reuse_cid1]]
            trids1 = [rid for rid in elements_cid_order[reuse_cid1]]

            # 可能只能复用一列
            both_retrieve = True
            # 可能会有新的一列需要取2个元素
            newline = False
            # 可能会有2列都需要取2个元素
            both_newline = True
            # 可能仅剩1个可复用列
            one_newline = False
            # 第一个周期的格式为: reuse_cid, reuse_cid, reuse_cid1, reuse_cid1
            # 如果只能复用一个 两者只需要一个大于0
            while column_length[reuse_cid] > 0 or column_length[reuse_cid1] > 0:
                # 如果少于2个 1元素块
                if len(one_element_block) == 1:
                    if both_retrieve:
                        # 如果剩余1个1元素块 且复用列中存在只有1个元素的列
                        if column_length[reuse_cid] == 1 or column_length[reuse_cid1] == 1:
                            color_map[target_color] = [config.elements_in_block]
                            pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order,
                                                              elements_rid_order, trids, reuse_cid, color_map,
                                                              target_color, column_length)

                            pop_one_element_from_reuse_column(trow_length1, row_length, elements_cid_order,
                                                              elements_rid_order, trids1, reuse_cid1, color_map,
                                                              target_color, column_length)
                            # 返回一个 1元素块
                            pop_one_element_block_and_update(elements_rid_order, row_length, capacity,
                                                             elements_cid_order,
                                                             one_element_block, color_map, target_color, column_length)
                            # pad 一个空元素
                            pad_one_element(color_map, target_color)
                            # 组号加1
                            target_color += 1

                            capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
                            capacity[reuse_cid1] = util.get_capacity(column_length[reuse_cid1])
                            break
                        else:
                            # 此时仅有1个 1元素块 与多个2/3 元素块 肯定可以调度
                            capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
                            capacity[reuse_cid1] = util.get_capacity(column_length[reuse_cid1])
                            break
                    else:
                        if len(two_element_block) > 0:
                            # 只从 reuse_cid列取数据
                            color_map[target_color] = [config.elements_in_block]
                            pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order,
                                                              elements_rid_order, trids, reuse_cid, color_map,
                                                              target_color, column_length)
                            # 依硬件实现， 先放2元素块
                            pop_two_element_block_and_update(elements_rid_order, row_length, capacity,
                                                             elements_cid_order,
                                                             two_element_block, color_map, target_color, column_length)
                            # 取1元素块
                            pop_one_element_block_and_update(elements_rid_order, row_length, capacity,
                                                             elements_cid_order,
                                                             one_element_block, color_map, target_color, column_length)
                            target_color += 1

                            capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
                            break
                        else:
                            # 没有2元素块，至多两个1元素块与多个3元素块，可以调度
                            capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
                            break
                elif len(one_element_block) == 0:
                    # 如果有0个1元素块
                    if both_retrieve:
                        # 如果有两个复用列 均只剩下一个元素 且有可接受的 2元素块
                        if column_length[reuse_cid] == 1 and column_length[reuse_cid1] == 1:
                            if len(two_element_block) > 0:
                                color_map[target_color] = [config.elements_in_block]
                                pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order,
                                                                  elements_rid_order, trids, reuse_cid, color_map,
                                                                  target_color, column_length)
                                pop_one_element_from_reuse_column(trow_length1, row_length, elements_cid_order,
                                                                  elements_rid_order, trids1, reuse_cid1, color_map,
                                                                  target_color, column_length)

                                pop_two_element_block_and_update(elements_rid_order, row_length, capacity,
                                                                 elements_cid_order,
                                                                 two_element_block, color_map, target_color,
                                                                 column_length)
                                target_color += 1

                                capacity[reuse_cid] = config.finish
                                capacity[reuse_cid1] = config.finish
                                break
                            else:
                                capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
                                capacity[reuse_cid1] = util.get_capacity(column_length[reuse_cid1])
                                break
                        else:
                            capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
                            capacity[reuse_cid1] = util.get_capacity(column_length[reuse_cid1])
                            break
                    else:
                        # 如果只有一个复用列
                        if column_length[reuse_cid] == 1:
                            if len(two_element_block) > 0:
                                color_map[target_color] = [config.elements_in_block]
                                pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order,
                                                                  elements_rid_order, trids, reuse_cid, color_map,
                                                                  target_color, column_length)

                                pop_two_element_block_and_update(elements_rid_order, row_length, capacity,
                                                                 elements_cid_order,
                                                                 two_element_block, color_map, target_color,
                                                                 column_length)
                                # pad 一个空元素
                                pad_one_element(color_map, target_color)

                                target_color += 1

                                capacity[reuse_cid] = config.finish
                                break
                            else:
                                capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
                                break
                        else:
                            capacity[reuse_cid] = util.get_capacity(column_length[reuse_cid])
                            break
                color_map[target_color] = [config.elements_in_block]
                if both_newline:
                    # 如果都需要取2个元素
                    both_newline = False
                    # 依次取两个元素
                    for _ in range(2):
                        pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order,
                                                          elements_rid_order, trids, reuse_cid, color_map,
                                                          target_color, column_length)

                    for _ in range(2):
                        pop_one_element_from_reuse_column(trow_length1, row_length, elements_cid_order,
                                                          elements_rid_order, trids1, reuse_cid1, color_map,
                                                          target_color, column_length)

                    # 组号加1
                    target_color += 1
                    color_map[target_color] = [config.elements_in_block]
                elif one_newline:
                    one_newline = False
                    # 取该列中所在行最长的两个元素
                    for _ in range(2):
                        pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order,
                                                          elements_rid_order, trids, reuse_cid, color_map,
                                                          target_color, column_length)
                    # 返回一个 1元素块
                    pop_one_element_block_and_update(elements_rid_order, row_length, capacity,
                                                     elements_cid_order,
                                                     one_element_block, color_map, target_color, column_length)
                    # pad 一个空元素
                    pad_one_element(color_map, target_color)

                    # 组号加1
                    target_color += 1
                    color_map[target_color] = [config.elements_in_block]

                # 第二个周期先取 reuse_cid 中的元素
                pop_one_element_from_reuse_column(trow_length, row_length, elements_cid_order,
                                                  elements_rid_order, trids, reuse_cid, color_map,
                                                  target_color, column_length)

                # 如果需要取第2列（reuse_cid1）中的元素
                if both_retrieve:
                    if newline:
                        newline = False
                        # 如果有新的列 则取第2列的2个元素
                        for _ in range(2):
                            pop_one_element_from_reuse_column(trow_length1, row_length, elements_cid_order,
                                                              elements_rid_order, trids1, reuse_cid1, color_map,
                                                              target_color, column_length)

                        pop_one_element_block_and_update(elements_rid_order, row_length, capacity,
                                                         elements_cid_order,
                                                         one_element_block, color_map, target_color, column_length)
                    else:
                        # 只取一个元素
                        pop_one_element_from_reuse_column(trow_length1, row_length, elements_cid_order,
                                                          elements_rid_order, trids1, reuse_cid1, color_map,
                                                          target_color, column_length)

                        # 小于2的情况已经预先处理， 肯定可以取到2个 1元素块
                        for _ in range(2):
                            pop_one_element_block_and_update(elements_rid_order, row_length, capacity,
                                                             elements_cid_order,
                                                             one_element_block, color_map, target_color, column_length)
                else:
                    for _ in range(2):
                        pop_one_element_block_and_update(elements_rid_order, row_length, capacity,
                                                         elements_cid_order,
                                                         one_element_block, color_map, target_color, column_length)

                    # 如果只取了1列，则需要pad一个元素
                    pad_one_element(color_map, target_color)

                target_color += 1

                # 如果均复用结束
                if column_length[reuse_cid] == 0 and column_length[reuse_cid1] == 0:
                    capacity[reuse_cid] = config.finish
                    capacity[reuse_cid1] = config.finish
                    # 如果已经没有列可以复用了，则退出
                    # both_retrieve 只在取不到新的复用列时为 False
                    if both_retrieve:
                        # 如果已经可以调度则退出
                        curr_capacity = util.sum_capacity(capacity)
                        if curr_capacity >= 0:
                            break
                        # 找新的复用列
                        reuseable_blocks = util.find_reuseable_columns(column_length)
                        # 若没有，则退出
                        if len(reuseable_blocks) == 0:
                            break
                        # 仅有一列, 置one_newline为True, both_retrieve为False
                        elif len(reuseable_blocks) == 1:
                            reuse_cid = reuseable_blocks[0]
                            trow_length = [row_length[rid] for rid in elements_cid_order[reuse_cid]]
                            trids = [rid for rid in elements_cid_order[reuse_cid]]

                            one_newline = True
                            both_retrieve = False
                        else:
                            # 置 both_newline为True
                            reuse_cid = reuseable_blocks[0]
                            trow_length = [row_length[rid] for rid in elements_cid_order[reuse_cid]]
                            trids = [rid for rid in elements_cid_order[reuse_cid]]
                            reuse_cid1 = reuseable_blocks[1]
                            trow_length1 = [row_length[rid] for rid in elements_cid_order[reuse_cid1]]
                            trids1 = [rid for rid in elements_cid_order[reuse_cid1]]

                            both_newline = True
                    else:
                        break
                elif column_length[reuse_cid] == 0:
                    # reuse_cid列已经用完
                    capacity[reuse_cid] = config.finish
                    # 如果还可能有可复用列， 否则将reuse_cid1的内容复制到reuse_cid中，
                    # 硬件设计的格式为: reuse_cid, index1, index2, pad
                    #                reuse_cid, reuse_cid1, reuse_cid1, index1
                    # both_retrieve 一定是 True
                    sum_capacity = util.sum_capacity(capacity, [reuse_cid1])
                    sum_capacity += column_length[reuse_cid1] * 2
                    # 如果已经可以调度， 则只消耗完当前列即可
                    if sum_capacity >= 0:
                        both_retrieve = False
                        trow_length = trow_length1
                        trids = trids1
                        reuse_cid = reuse_cid1
                    if both_retrieve:
                        reuseable_blocks = util.find_reuseable_columns(column_length, 1, current_reuse=[reuse_cid1])
                        if len(reuseable_blocks) == 1:
                            # 如果取到新的可复用列， 置 newline 为True， 将reuse_cid1的内容复制到reuse_cid中
                            newline = True
                            trow_length = trow_length1
                            trids = trids1
                            reuse_cid = reuse_cid1

                            reuse_cid1 = reuseable_blocks[0]
                            trow_length1 = [row_length[rid] for rid in elements_cid_order[reuse_cid1]]
                            trids1 = [rid for rid in elements_cid_order[reuse_cid1]]
                        else:
                            # 将reuse_cid1的内容复制到reuse_cid中
                            # 格式为 reuse_cid, index1, index2, pad
                            trow_length = trow_length1
                            trids = trids1
                            reuse_cid = reuse_cid1
                            both_retrieve = False
                elif column_length[reuse_cid1] == 0:
                    # reuse_cid1列已经用完
                    capacity[reuse_cid1] = config.finish
                    sum_capacity = util.sum_capacity(capacity, [reuse_cid])
                    sum_capacity += column_length[reuse_cid] * 2
                    # 如果已经可以调度， 则只消耗完当前列即可
                    if sum_capacity >= 0:
                        both_retrieve = False
                    if both_retrieve:
                        reuseable_blocks = util.find_reuseable_columns(column_length, 1, current_reuse=[reuse_cid])
                        if len(reuseable_blocks) == 1:
                            newline = True
                            reuse_cid1 = reuseable_blocks[0]
                            trow_length1 = [row_length[rid] for rid in elements_cid_order[reuse_cid1]]
                            trids1 = [rid for rid in elements_cid_order[reuse_cid1]]
                        else:
                            both_retrieve = False

    # 可以按行主序常规调度， 不需要复用或者不存在复用
    while elements_counter > 0:
        color_map[target_color] = [config.elements_in_block]

        # 如果少于4个元素， 直接结束
        if elements_counter <= 4:
            color_map[target_color][0] = 0
            for cid in elements_cid_order.keys():
                for rid in elements_cid_order[cid]:
                    color_map[target_color].append([cid, rid])
            # pad some elements
            for _ in range(4 - elements_counter):
                color_map[target_color].append([color_map[target_color][-1][0], config.pad_rid])
            break

        # 取最长行的一个元素
        target_cid, target_rid, opcode = util.pop_one_element(elements_rid_order, row_length,
                                                              elements_cid_order, capacity)

        # 如果是 1元素块
        if opcode == config.single_element:
            update_parameters(row_length, elements_cid_order, elements_rid_order,
                              target_rid, target_cid, color_map, target_color, column_length)
            capacity[target_cid] = config.finish

            # 如果存在 3元素块
            if util.have_capacity(capacity):
                pop_three_elements_wr(elements_rid_order, row_length, capacity, elements_cid_order,
                                      config.single_element, color_map, target_color, column_length)
            else:
                # 如果没有 3元素块
                possible_cids = util.have_twoelement_block(column_length)
                # 只有 2元素块 与 1元素块
                temp_len = elements_counter - len(possible_cids) * 2
                # 如果当前 2元素块 的个数为偶数， 则不需要去拆
                if len(possible_cids) % 2 == 0:
                    # 如果剩余的 1元素块 数量少于3个 则需要pad
                    if temp_len < 3:
                        for _ in range(temp_len):
                            pop_one_element_wr(elements_rid_order, row_length, capacity, elements_cid_order,
                                               color_map, target_color, column_length)
                        for _ in range(3 - temp_len):
                            pad_one_element(color_map, target_color)
                    else:
                        for _ in range(3):
                            pop_one_element_wr(elements_rid_order, row_length, capacity, elements_cid_order,
                                               color_map, target_color, column_length)
                else:
                    # 奇数个 2元素块 则消耗当前的元素 先放置2元素块
                    target_cid, target_rids = \
                        util.find_component(elements_rid_order, row_length, capacity,
                                            elements_cid_order, config.two_element, possible_cids)
                    for rid in target_rids:
                        update_parameters(row_length, elements_cid_order, elements_rid_order,
                                          rid, target_cid, color_map, target_color, column_length)
                    # 只有4元素块 与 2元素块
                    if column_length[target_cid] == 0:
                        capacity[target_cid] = config.finish
                    else:
                        capacity[target_cid] = config.two_element

                    # 硬件设计 保证相同的cid 在 12 34位
                    temp = color_map[target_color][1]
                    color_map[target_color][1] = color_map[target_color][2]
                    color_map[target_color][2] = color_map[target_color][3]
                    color_map[target_color][3] = temp

                    # pad 1个元素
                    pad_one_element(color_map, target_color)
        # 如果是 2元素块
        elif opcode == config.two_element:
            curr_capacity = util.sum_capacity(capacity)
            # 由于考虑我们可能将其分开， 先将一个元素放入
            update_parameters(row_length, elements_cid_order, elements_rid_order,
                              target_rid[0], target_cid, color_map, target_color, column_length)
            # 只有可能是 2元素块 或者 4元素块
            if column_length[target_cid] == 1:
                capacity[target_cid] = config.single_element
            else:
                capacity[target_cid] = 1
            possible_cids = util.have_twoelement_block(column_length)
            if possible_cids:
                # 如果有 2元素块, 先将剩下的元素放入
                update_parameters(row_length, elements_cid_order, elements_rid_order,
                                  target_rid[1], target_cid, color_map, target_color, column_length)
                if column_length[target_cid] == 0:
                    capacity[target_cid] = config.finish
                elif column_length[target_cid] == 2:
                    capacity[target_cid] = config.two_element
                target_cid, target_rids = util.find_component(elements_rid_order, row_length,
                                                              capacity, elements_cid_order, opcode, possible_cids)
                for rid in target_rids:
                    update_parameters(row_length, elements_cid_order, elements_rid_order,
                                      rid, target_cid, color_map, target_color, column_length)
                # 如果有 2元素块 只有一种情况可能改变其 capacity
                if column_length[target_cid] == 0:
                    capacity[target_cid] = config.finish
            else:
                # 如果没有 2元素块
                if curr_capacity > 0:
                    # 如果有 3元素块
                    pop_three_elements_wr(elements_rid_order, row_length, capacity, elements_cid_order,
                                          config.single_element, color_map, target_color, column_length)
                else:
                    # 如果没有 3元素块
                    update_parameters(row_length, elements_cid_order, elements_rid_order,
                                      target_rid[1], target_cid, color_map, target_color, column_length)
                    if column_length[target_cid] == 0:
                        capacity[target_cid] = config.finish
                    elif column_length[target_cid] == 2:
                        capacity[target_cid] = config.two_element

                    # 取 1元素块 pad 一个元素
                    pop_one_element_wr(elements_rid_order, row_length, capacity, elements_cid_order,
                                       color_map, target_color, column_length)

                    pad_one_element(color_map, target_color)
        # 如果是 3元素块
        elif opcode == config.have_capacity:
            for rid in target_rid:
                update_parameters(row_length, elements_cid_order, elements_rid_order,
                                  rid, target_cid, color_map, target_color, column_length)
            capacity[target_cid] -= 1
            if column_length[target_cid] == 0:
                capacity[target_cid] = config.finish

            # 如果有 1元素块
            if config.single_element in capacity.values():
                # if there is a consumer, consumes
                pop_one_element_wr(elements_rid_order, row_length, capacity, elements_cid_order,
                                   color_map, target_color, column_length)
            else:
                # 如果有 2元素块 拆开
                possible_cids = util.have_twoelement_block(column_length)
                target_cid = 0
                if possible_cids:
                    target_cid, target_rid = \
                        util.find_component(elements_rid_order, row_length,
                                            capacity, elements_cid_order, config.two_element, possible_cids)
                    max_rid = target_rid[0] if row_length[target_rid[0]] > row_length[target_rid[1]] else target_rid[1]
                    update_parameters(row_length, elements_cid_order, elements_rid_order,
                                      max_rid, target_cid, color_map, target_color, column_length)
                else:
                    # 从最长的行选一个
                    values = list(row_length.values())
                    max_rid = list(row_length.keys())[values.index(max(values))]
                    target_cid = elements_rid_order[max_rid][0]
                    update_parameters(row_length, elements_cid_order, elements_rid_order,
                                      max_rid, target_cid, color_map, target_color, column_length)
                # 更新 capacity
                curr_cid_time = column_length[target_cid]
                capacity[target_cid] = util.get_capacity(curr_cid_time)

        # 组号加1
        target_color += 1

    with open(target_folder + "/" + output_file + '.txt', mode='a+') as wfile:
        wfile.write("the {}th of output_file {}\n".format(batch_index + 1, output_file))
        for value in color_map.values():
            wfile.write(str(value) + '\n')

    return


def main():
    if not os.path.exists(os.getcwd() + "/" + target_folder):
        os.mkdir(target_folder)

    if not os.path.exists(os.getcwd() + "/" + config.target_bin_folder):
        os.mkdir(config.target_bin_folder)

    for tfile in os.listdir(benchmark_folder):
        # the unzipped benchmark folder
        if os.path.isdir(benchmark_folder + "/" + tfile):
            if os.path.exists(target_folder + "/" + tfile + ".txt"):
                os.remove(target_folder + "/" + tfile + ".txt")
            s_path = benchmark_folder + "/{}/{}.mtx".format(tfile, tfile)
            rid, cid, val, rows = data.initialize_matrix_data(s_path)
            print("----------------------------------")
            print("{} has {} rows".format(tfile, rows))
            logger.info("{} has {} rows".format(tfile, rows))
            logger.info("----------------------------------")
            # row_batch数量
            total_batch = rows // config.rows if rows % config.rows == 0 else rows // config.rows + 1
            for i in range(total_batch):
                print("the {}th r_batch of {}, total process {:.2f}\n".format(i+1, tfile, (i+1)/total_batch))
                mapping_algorithm(rid, cid, val, i, tfile)
            
            write_to_BIN.write_to_BIN(target_folder + '/' + tfile + '.txt', logger)
            logger.info("Totally {} rows".format(rows))
            logger.info("The final vector length is {}".format(rows + 8 - rows % 8))
            print("----------------------------------")
            logger.info("----------------------------------")


if __name__ == '__main__':
    main()
