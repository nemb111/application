pub fn factors(n: u64) -> Vec<u64> {
    let mut res = Vec::new();
    let mut n_copy = n;

    for i in 2..=n {
        while 0 == n_copy % i && n_copy != 0 {
            n_copy /= i;
            res.push(i);
        }

        if i > n_copy {
            break;
        }
    }

    res
}
