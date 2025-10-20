use std::collections::HashSet;

pub fn find_saddle_points(input: &[Vec<u64>]) -> Vec<(usize, usize)> {
    let col_size = input.len();
    let row_size = input[0].len();

    let mut col_candidates: HashSet<(usize, usize)> = HashSet::with_capacity(col_size * row_size);
    let mut row_candidates: HashSet<(usize, usize)> = HashSet::with_capacity(col_size * row_size);

    col_candidates = get_col_candidates(col_candidates, input, col_size, row_size);
    row_candidates = get_row_candidates(row_candidates, input, col_size, row_size);

    col_candidates
        .intersection(&row_candidates)
        .copied()
        .collect()
}

fn get_col_candidates(
    mut col_candidates: HashSet<(usize, usize)>,
    input: &[Vec<u64>],
    col_size: usize,
    row_size: usize,
) -> HashSet<(usize, usize)> {
    col_candidates.clear();

    let mut one_col_candidates: HashSet<(usize, usize)> = HashSet::with_capacity(col_size);
    for row in 0..row_size {
        one_col_candidates.clear();
        let mut smallest_el = u64::max_value();
        for col in 0..col_size {
            if input[col][row] < smallest_el {
                smallest_el = input[col][row];
                one_col_candidates.clear();
                one_col_candidates.insert((col, row));
            } else if input[col][row] == smallest_el {
                one_col_candidates.insert((col, row));
            }
        }
        col_candidates = col_candidates.union(&one_col_candidates).copied().collect();
    }

    col_candidates
}

fn get_row_candidates(
    mut row_candidates: HashSet<(usize, usize)>,
    input: &[Vec<u64>],
    col_size: usize,
    row_size: usize,
) -> HashSet<(usize, usize)> {
    row_candidates.clear();

    let mut one_row_candidates: HashSet<(usize, usize)> = HashSet::with_capacity(row_size);
    for col in 0..col_size {
        one_row_candidates.clear();
        let mut largest_el = u64::min_value();
        for row in 0..row_size {
            if input[col][row] > largest_el {
                largest_el = input[col][row];
                one_row_candidates.clear();
                one_row_candidates.insert((col, row));
            } else if input[col][row] == largest_el {
                one_row_candidates.insert((col, row));
            }
        }
        row_candidates = row_candidates.union(&one_row_candidates).copied().collect();
    }

    row_candidates
}
