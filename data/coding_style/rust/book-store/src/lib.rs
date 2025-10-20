pub fn lowest_price(books: &[u32]) -> u32 {
    let mut amount = vec![0, 0, 0, 0, 0];
    let mut sets = vec![0, 0, 0, 0, 0];
    for i in books {
        amount[(*i - 1) as usize] += 1;
    }
    amount.sort_unstable_by_key(|&b| std::cmp::Reverse(b));

    while !amount.eq(&[0, 0, 0, 0, 0]) {
        let group_size = amount.iter().filter(|i| **i > 0).count();
        for i in amount.iter_mut().take(group_size) {
            *i -= 1;
        }
        sets[group_size - 1] += 1;
    }

    while sets[4] > 0 && sets[2] > 0 {
        sets[4] -= 1;
        sets[2] -= 1;
        sets[3] += 1;
        sets[3] += 1;
    }

    sets.iter()
        .enumerate()
        .map(|(i, v)| v * discount_mapping(i + 1))
        .sum()
}

fn discount_mapping(group_size: usize) -> u32 {
    match group_size {
        5 => 5 * 600,
        4 => 4 * 640,
        3 => 3 * 720,
        2 => 2 * 760,
        1 => 800,
        _ => unreachable!(),
    }
}
