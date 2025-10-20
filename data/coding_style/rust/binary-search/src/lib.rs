pub fn find(array: &[i32], key: i32) -> Option<usize> {
    if array.is_empty() {
        return None;
    }
    let idx = array.len() / 2;
    find_in_range(array, idx, 0usize, array.len(), key)
}

pub fn find_in_range(
    array: &[i32],
    idx: usize,
    start: usize,
    end: usize,
    key: i32,
) -> Option<usize> {
    if array[idx] == key {
        return Some(idx);
    } else if end - start == 1 {
        return None;
    } else if array[idx] > key {
        let new_idx = start + (idx - start) / 2;
        return find_in_range(array, new_idx, start, idx, key);
    } else {
        let new_idx = idx + (end - idx) / 2;
        find_in_range(array, new_idx, idx, end, key)
    }
}
