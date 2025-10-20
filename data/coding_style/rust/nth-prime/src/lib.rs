pub fn nth(n: u32) -> u32 {
    let mut i = 2;
    let mut vec = Vec::new();
    vec.push(2);

    while vec.len() <= (n + 1) as usize {
        i += 1;

        if vec.iter().all(|&x| i % x != 0) {
            vec.push(i);
        }
    }

    vec[n as usize]
}
