pub fn is_armstrong_number(num: u32) -> bool {
    let s = num.to_string();
    let c_vec: Vec<char> = s.chars().collect();

    let result = c_vec
        .iter()
        .map(|x| x.to_digit(10u32).unwrap())
        .map(|x| x.pow(c_vec.len() as u32))
        .sum::<u32>();

    num == result
}
