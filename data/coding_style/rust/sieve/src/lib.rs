use std::convert::TryInto;

pub fn primes_up_to(upper_bound: u64) -> Vec<u64> {
    let mut res = Vec::new();
    let mut sieve = vec![false; upper_bound as usize + 1usize];

    for i in 2..=upper_bound {
        if !sieve[i as usize] {
            res.push(i);
            for j in (i..=upper_bound).step_by(i.try_into().unwrap()) {
                sieve[j as usize] = true;
            }
        }
    }

    res
}
