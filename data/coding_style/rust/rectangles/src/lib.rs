use itertools::Itertools;

fn find_connected_top_right_points(byte_arrays: &[&[u8]], row: usize, col: usize) -> Vec<usize> {
    let mut top_right_points = vec![];
    for j in col + 1..byte_arrays[row].len() {
        if byte_arrays[row][j] == b'-' {
            continue;
        } else if byte_arrays[row][j] == b'+' {
            top_right_points.push(j);
        } else {
            break;
        }
    }

    top_right_points
}

fn find_connected_bottom_left_points(byte_arrays: &[&[u8]], row: usize, col: usize) -> Vec<usize> {
    let mut bottom_left_points = vec![];
    for (i, &el) in byte_arrays.iter().enumerate().skip(row + 1) {
        if el[col] == b'|' {
            continue;
        } else if el[col] == b'+' {
            bottom_left_points.push(i);
        } else {
            break;
        }
    }

    bottom_left_points
}

fn is_connected(
    byte_arrays: &[&[u8]],
    row: usize,
    col: usize,
    bottom_row: usize,
    right_col: usize,
) -> bool {
    if byte_arrays[bottom_row][right_col] != b'+' {
        return false;
    }

    for &el in byte_arrays.iter().take(bottom_row).skip(row + 1) {
        if !(el[right_col] == b'|' || el[right_col] == b'+') {
            return false;
        }
    }

    for j in col + 1..right_col {
        if !(byte_arrays[bottom_row][j] == b'-' || byte_arrays[bottom_row][j] == b'+') {
            return false;
        }
    }

    true
}

pub fn count(lines: &[&str]) -> u32 {
    let byte_arrays: Vec<&[u8]> = lines.iter().map(|line| line.as_bytes()).collect();

    let n_rows = byte_arrays.len();
    let n_cols = byte_arrays.first().map_or(0, |line| line.len());

    let mut number_of_rectangles = 0u32;

    for row in 0..n_rows {
        for col in 0..n_cols {
            if byte_arrays[row][col] == b'+' {
                let top_right_points: &[usize] =
                    &find_connected_top_right_points(&byte_arrays, row, col);
                let bottom_left_points: &[usize] =
                    &find_connected_bottom_left_points(&byte_arrays, row, col);

                for point_to_check in bottom_left_points
                    .iter()
                    .cartesian_product(top_right_points)
                {
                    if is_connected(&byte_arrays, row, col, *point_to_check.0, *point_to_check.1) {
                        number_of_rectangles += 1;
                    }
                }
            }
        }
    }

    number_of_rectangles
}
