import collections

import config
import struct
import copy


def map_dict_add(target: dict, element: dict):
    curr_key = list(element.keys())[0]
    rid_key = list(element[curr_key].keys())[0]
    if curr_key in target.keys():
        target[curr_key].append(rid_key)
    else:
        target[curr_key] = []
        target[curr_key].append(rid_key)


def counter_dict_add(target: dict, element):
    if element in target.keys():
        target[element] += 1
    else:
        target[element] = 1


def get_capacity(counter: int):
    # 不考虑组间复用的情况下 当前列的capacity，即有多少个 3元素块
    target_capacity = 0
    if counter % 3 == 0:
        # 返回 3元素块 的个数
        if counter == 0:
            target_capacity = config.finish
        else:
            target_capacity = counter // 3
    elif counter % 3 == 1:
        if counter == 1:
            # 1元素块
            target_capacity = config.single_element
        elif counter == 4:
            # 4个元素会被分成 2个 2元素块 因为分成1个与3个并没有区别，2个 2元素块 仍会考虑到行元素
            target_capacity = config.two_element
        else:
            # 分为1个4元素块与 多个 3元素块
            target_capacity = counter // 3 - 1
    elif counter % 3 == 2:
        # 分为1个2元素块 与 多个3元素块
        target_capacity = counter // 3
    return target_capacity


def sum_capacity(capacity: collections.OrderedDict, forbidden_one=None):
    t_sum = 0

    if forbidden_one is None:
        forbidden_one = []

    for key, item in capacity.items():
        if key not in forbidden_one:
            if item != config.finish:
                t_sum += item
    return t_sum


def have_capacity(capacity: collections.OrderedDict):
    for item in capacity.values():
        if item > 0:
            return True
    return False


def find_reuseable_columns(column_map: collections.OrderedDict, target_number=2, current_reuse=None):
    # 返回最长的列，如果设置target_number, 只返回一列
    target_cids = []
    target_cids_counter = []

    if current_reuse is None:
        current_reuse = []

    for cid, counter in column_map.items():
        # 只有多于4个元素的列才可以被复用
        if counter >= 4:
            if cid not in current_reuse:
                # 如果已经满了 替换掉元素少的
                if len(target_cids) < target_number:
                    target_cids.append(cid)
                    target_cids_counter.append(counter)
                else:
                    min_index = target_cids_counter.index(min(target_cids_counter))
                    if counter > target_cids_counter[min_index]:
                        target_cids[min_index] = cid
                        target_cids_counter[min_index] = counter
    return target_cids


def have_twoelement_block(column_map: collections.OrderedDict):
    cids = []
    for key, value in column_map.items():
        if value == 2 or value % 3 == 2:
            # 2元素块
            cids.append(key)
        elif value % 3 == 1 and value != 1:
            # 4元素块
            cids += [key, key]
    return cids


def have_oneelement_block(column_map: collections.OrderedDict):
    cids = []
    for key, value in column_map.items():
        if value == 1:
            # 1元素块
            cids.append(key)
    return cids


def pop_one_element(elements_rid_map: collections.OrderedDict, row_length: collections.OrderedDict,
                    element_cid_map: collections.OrderedDict, capacity: collections.OrderedDict):
    """
        pop one element from the existing longest row
    """
    values = list(row_length.values())
    max_rid = list(row_length.keys())[values.index(max(values))]

    # 最长行的 capacity
    capacity_curr_rid = collections.OrderedDict()
    for cid in elements_rid_map[max_rid]:
        capacity_curr_rid[cid] = capacity[cid]

    values = list(capacity_curr_rid.values())
    # 优先返回 1元素块
    if config.single_element in values:
        final_cid = list(capacity_curr_rid.keys())[values.index(config.single_element)]
        return final_cid, max_rid, config.single_element
    # 如果没有 1元素块， 返回行最长的 2元素块
    elif config.two_element in values:
        max_two_element = 0
        max_two_element_rid = 0
        final_cid = 0

        for cid in capacity_curr_rid.keys():
            if capacity_curr_rid[cid] == config.two_element:
                for rid in element_cid_map[cid]:
                    if rid != max_rid:
                        if row_length[rid] > max_two_element:
                            max_two_element = row_length[rid]
                            max_two_element_rid = rid
                            final_cid = cid
        return final_cid, [max_rid, max_two_element_rid], config.two_element
    else:
        # 返回 3元素块
        max_three_element = []
        max_three_element_rid = []
        final_cid = 0
        for cid in capacity_curr_rid.keys():
            temp_element = []
            temp_element_rid = []
            for rid in element_cid_map[cid]:
                if rid != max_rid:
                    if len(temp_element_rid) < 2:
                        temp_element_rid.append(rid)
                        temp_element.append(row_length[rid])
                    else:
                        min_length = min(temp_element)
                        if row_length[rid] > min_length:
                            replace_index = temp_element.index(min_length)
                            temp_element[replace_index] = row_length[rid]
                            temp_element_rid[replace_index] = rid
            if sum(temp_element) > sum(max_three_element):
                final_cid = cid
                max_three_element_rid = temp_element_rid
                max_three_element = temp_element
        return final_cid, max_three_element_rid + [max_rid], config.have_capacity


def find_component(elements_rid_map: collections.OrderedDict, row_length: collections.OrderedDict,
                   capacity: collections.OrderedDict, elements_cid_map: collections.OrderedDict, opcode: int,
                   possible_cids: list):
    if opcode == config.single_element:
        # 需要找到1个 3元素块
        values = list(row_length.values())
        find = False
        target_rid = 0
        while not find:
            curr_max_index = values.index(max(values))
            max_rid = list(row_length.keys())[curr_max_index]
            for cid in elements_rid_map[max_rid]:
                if capacity[cid] > 0:
                    target_rid = max_rid
                    find = True
                    break
            values[curr_max_index] = -2
        # find the three rids in that column
        capacity_curr_rid = collections.OrderedDict()
        for cid in elements_rid_map[target_rid]:
            capacity_curr_rid[cid] = capacity[cid]

        max_three_element = []
        max_three_element_rid = []
        final_cid = 0
        for cid in capacity_curr_rid.keys():
            if capacity_curr_rid[cid] > 0:
                temp_element = []
                temp_element_rid = []
                for rid in elements_cid_map[cid]:
                    if rid != target_rid:
                        if len(temp_element_rid) < 2:
                            temp_element_rid.append(rid)
                            temp_element.append(row_length[rid])
                        else:
                            min_length = min(temp_element)
                            if row_length[rid] > min_length:
                                replace_index = temp_element.index(min_length)
                                temp_element[replace_index] = row_length[rid]
                                temp_element_rid[replace_index] = rid
                if sum(temp_element) > sum(max_three_element):
                    final_cid = cid
                    max_three_element_rid = temp_element_rid
                    max_three_element = temp_element
        return final_cid, max_three_element_rid + [target_rid]
    elif opcode == config.two_element:
        # 如果需要找到 2元素块
        values = list(row_length.values())
        find = False
        target_rid = 0
        intersetcids = []
        while not find:
            max_rid = list(row_length.keys())[values.index(max(values))]
            intersetcids = list(set(possible_cids).intersection(set(elements_rid_map[max_rid])))
            if intersetcids:
                find = True
                target_rid = max_rid
            values[values.index(max(values))] = -2
        max_two_element = 0
        max_two_element_rid = 0
        final_cid = 0
        for cid in intersetcids:
            for rid in elements_cid_map[cid]:
                if rid != target_rid:
                    if row_length[rid] > max_two_element:
                        max_two_element = row_length[rid]
                        max_two_element_rid = rid
                        final_cid = cid
        return final_cid, [max_two_element_rid, target_rid]
    elif opcode == config.find_one:
        # 如果需要找到 1元素块
        values = list(row_length.values())
        find = False
        target_rid = 0
        target_cid = 0
        while not find:
            max_rid = list(row_length.keys())[values.index(max(values))]
            capacity_curr_rid = collections.OrderedDict()
            for cid in elements_rid_map[max_rid]:
                capacity_curr_rid[cid] = capacity[cid]
            curr_values = list(capacity_curr_rid.values())
            if config.single_element in curr_values:
                find = True
                target_rid = max_rid
                target_cid = list(capacity_curr_rid.keys())[curr_values.index(config.single_element)]
            values[values.index(max(values))] = -2
        return target_cid, target_rid


def write_to_bin_double(path: str, obj: list):
    with open(path, 'wb') as wfile:
        for i in range(len(obj)):
            wfile.write(struct.pack('d', obj[i]))


def write_to_bin_unsignedint(path: str, obj: list):
    with open(path, 'wb') as wfile:
        for i in range(len(obj)):
            wfile.write(struct.pack('I', obj[i]))


def write_to_bin_unsignedlong(path: str, obj: list):
    with open(path, 'wb') as wfile:
        for i in range(len(obj)):
            wfile.write(struct.pack('Q', obj[i]))
