import util
import logging
import config
import os


def write_to_BIN(textfile: str, logger: logging.Logger):
    matrix1 = []
    matrix2 = []
    rid = []
    cid = []
    row_length = []

    counter = 0

    with open(textfile, 'r') as rfile:
        line = rfile.readline()
        while line:
            line = line.strip()
            if line.startswith('t'):
                if counter != 0:
                    if counter < 32:
                        for _ in range(32 - counter):
                            matrix1 += [1.0] * 2
                            matrix2 += [1.0] * 2
                            rid += [config.pad_rid] * 4
                            cid += [0] * 4
                        counter = 32
                    row_length.append(counter)
                counter = 0
            else:
                line = line.split('[')
                for item in line[2: -1]:
                    curr_cid = int(item[: item.find(',')])
                    curr_rid = int(item[item.find(',')+2: -3])
                    cid.append(curr_cid)
                    rid.append(curr_rid)
                item = line[-1]
                curr_cid = int(item[:item.find(',')])
                curr_rid = int(item[item.find(',')+2:-2])
                cid.append(curr_cid)
                rid.append(curr_rid)
                counter += 1
                matrix1 += [1.0] * 2
                matrix2 += [1.0] * 2
            line = rfile.readline()
        if counter != 0:
            if counter < 32:
                for _ in range(32 - counter):
                    matrix1 += [1.0] * 2
                    matrix2 += [1.0] * 2
                    rid += [config.pad_rid] * 4
                    cid += [0] * 4
                counter = 32
            row_length.append(counter)
        
        if not os.path.exists(config.target_bin_folder + textfile[textfile.find('/')+1: textfile.find('.')]):
            os.mkdir(config.target_bin_folder + textfile[textfile.find('/')+1: textfile.find('.')])
        util.write_to_bin_double(config.target_bin_folder + textfile[textfile.find('/')+1: textfile.find('.')] + '/val1.BIN', matrix1)
        util.write_to_bin_double(config.target_bin_folder + textfile[textfile.find('/')+1: textfile.find('.')] + '/val2.BIN', matrix2)
        util.write_to_bin_unsignedint(config.target_bin_folder + textfile[textfile.find('/')+1: textfile.find('.')] + '/rid.BIN', rid)
        util.write_to_bin_unsignedint(config.target_bin_folder + textfile[textfile.find('/')+1: textfile.find('.')] + '/cid.BIN', cid)
        util.write_to_bin_unsignedint(config.target_bin_folder + textfile[textfile.find('/')+1: textfile.find('.')] + '/rowl.BIN', row_length)
    
    logger.info("------- Summary of {} --------".format(textfile[:textfile.find('.')]))
    logger.info("Totally {} batches".format(len(row_length)))
    logger.info("Totally {} groups".format(len(matrix1) // 2))


if __name__ == '__main__':
    if not os.path.exists(config.target_bin_folder):
        os.mkdir(config.target_bin_folder)
    write_to_BIN('output/torso2.txt', logging.getLogger())
