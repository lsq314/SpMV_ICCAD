from scipy.io import mmread
import numpy as np
import collections


def initialize_matrix_data(path: str):
    origin_matrix = mmread(path).tocsc()

    origin_rid = origin_matrix.indices.tolist()
    origin_cid = get_all_cid(origin_matrix.indptr)
    origin_val = origin_matrix.data.tolist()

    return origin_rid, origin_cid, origin_val, len(origin_matrix.indptr) - 1


def get_all_cid(original_ptr: np.ndarray):
    col = list()
    temp_index = 0
    for i in range(len(original_ptr) - 1):
        if original_ptr[i + 1] != original_ptr[i]:
            for j in range(original_ptr[i + 1] - original_ptr[i]):
                col.append(temp_index)
            temp_index += 1
        else:
            temp_index += 1
    return col
