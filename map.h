#include <array>
#include <iostream>
#include <assert.h>

template <unsigned int num_symbols, unsigned int num_msgs, unsigned int num_rows, unsigned int num_cols, bool set_left_cols>
class Map {
public:
    void init() {
        std::array<unsigned int, num_msgs> vals;
        vals.fill(0);
        init(vals);
    }

    void print(unsigned int cells = num_rows * num_cols) const {
        for (unsigned int i = 0; i < num_rows; i++) {
            for (unsigned int j = 0; j < num_cols; j++) {
                std::cout << data[i][j] << " ";
                if (!--cells) {
                    std::cout << std::endl;
                    return;
                }
            }
            std::cout << std::endl;
        }
    }

    template <unsigned int eval_pos>
    bool run(unsigned int rank, unsigned int max_rank, unsigned int cell_start) {
        this->rank = rank;
        this->max_rank = max_rank;
        this->cell_start = cell_start;

        assert(cell_start < eval_pos);

        if (set_left_cols) {
            assert((cell_start % num_cols) >= num_symbols);
        }

        return run_descend<eval_pos>(cell_start);
    }

private:
    std::array<std::array<unsigned int, num_cols>, num_rows> data;

    unsigned int rank;
    unsigned int max_rank;
    unsigned int cell_start;

    template <unsigned int cell_end>
    unsigned int calc_target_rank() const {
        unsigned int hash = 0;
        for (unsigned int i = cell_start; i < cell_end; i++) {
            unsigned int row = i / num_cols;
            unsigned int col = i % num_cols;
            hash *= num_symbols;
            hash += data[row][col];
        }

        hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
        hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
        hash = (hash >> 16) ^ hash;

        return hash % max_rank;
    }

    void init(std::array<unsigned int, num_msgs> &vals) {
        for (unsigned int i = 0; i < num_cols; i++) {
            for (unsigned int j = 0; j < num_msgs; j++) {
                data[j][i] = vals[j];
            }

            unsigned int j = num_msgs - 1;
            while (vals[j] == num_symbols - 1) {
                vals[j] = 0;
                if (j == 0) {break;}
                j--;
            }
            vals[j]++;
        }

        if (set_left_cols) {
            for (unsigned int i = num_msgs; i < num_rows; i++) {
                for (unsigned int j = 0; j < num_symbols; j++) {
                    data[i][j] = j;
                }
            }
        }
    }

    template <unsigned int eval_pos>
    bool run_descend(unsigned int cell) {
        //print_grid(grid);
        //usleep(1);

        if (cell == eval_pos) {
            if (calc_target_rank<eval_pos>() != rank) {return false;}
        }

        if (cell == num_rows * num_cols) {return true;}

        unsigned int row = cell / num_cols;
        unsigned int col = cell % num_cols;

        if (set_left_cols) {
            while (col < num_symbols) {
                cell++;
                col++;
            }
        }

        for (unsigned int i = 0; i < num_symbols; i++) {
            data[row][col] = i;
            if (is_valid(row, col)) {
                if (run_descend<eval_pos>(cell + 1)) {return true;}
            }
        }

        return false;
    }

    bool is_valid(unsigned int row, unsigned int col) {
        assert(row >= num_msgs);

        std::array<unsigned int, num_msgs> rows;
        rows[num_msgs - 1] = row;

        for (unsigned int i = 0; i < col; i++) {
            if (data[row][i] == data[row][col]) {
                if (!is_valid(rows, i, col, num_msgs - 1)) {return false;}
            }
        }

        return true;
    }

    bool is_valid(std::array<unsigned int, num_msgs> &rows, unsigned int left_col, unsigned int right_col, unsigned int row_i) const {
        if (row_i == 0) {
            for (unsigned int i = 0; i < num_msgs - 1; i++) {
                const std::array<unsigned int, num_cols> &row = data[rows[i]];
                if (row[left_col] != row[right_col]) {return true;}
            }
            return false;
        }

        unsigned int row_max = rows[row_i];
        row_i--;
        for (unsigned int i = row_i; i < row_max; i++) {
            rows[row_i] = i;
            if (!is_valid(rows, left_col, right_col, row_i)) {return false;}
        }

        return true;
    }
};
