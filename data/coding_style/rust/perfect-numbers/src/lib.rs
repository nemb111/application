#[derive(Debug, PartialEq, Eq)]
pub enum Classification {
    Abundant,
    Perfect,
    Deficient,
}

pub fn classify(num: u64) -> Option<Classification> {
    if 0 == num {
        return None;
    }

    let aliquot_sum: u64 = (1..num).filter(|n| num % n == 0).sum();

    match aliquot_sum {
        n if n == num => Some(Classification::Perfect),
        n if n > num => Some(Classification::Abundant),
        n if n < num => Some(Classification::Deficient),
        _ => unreachable!(),
    }
}
