pub fn egg_count(display_value: u32) -> usize {
    display_value
        .to_ne_bytes()
        .iter()
        .map(|&b| b.count_ones() as usize)
        .sum()
}
