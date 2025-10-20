pub fn encode(n: u64) -> String {
    if n == 0 {
        return String::from("zero");
    }

    let mut res: String = String::new();
    // iterate over packs of thousand and insert appropriate scale word
    let mut n = n;
    let mut nth_pack_of_thousand = 0;
    loop {
        let hundreds_tens_ones = n % 1000_u64;

        if hundreds_tens_ones != 0 {
            let hundreds_tens_ones_stringified =
                stringify_hundreds_tens_and_ones(hundreds_tens_ones);
            let nth_pack_of_thousand_stringified =
                stringify_nth_pack_of_thousand(nth_pack_of_thousand);

            res = (hundreds_tens_ones_stringified
                + " "
                + nth_pack_of_thousand_stringified
                + " "
                + &res)
                .trim()
                .to_string();
        }

        n /= 1000;
        nth_pack_of_thousand += 1;

        if n == 0 {
            break;
        }
    }

    res
}

fn stringify_hundreds_tens_and_ones(n: u64) -> String {
    debug_assert!(n <= 999);

    let digit_hundreds = n / 100;
    let digits_tens_and_ones = n % 100;

    match digit_hundreds {
        0 => tens(digits_tens_and_ones),
        _ => (tens(digit_hundreds) + " hundred " + &tens(digits_tens_and_ones))
            .trim()
            .to_string(),
    }
}

fn stringify_nth_pack_of_thousand(n: u64) -> &'static str {
    match n {
        0 => "",
        1 => "thousand",
        2 => "million",
        3 => "billion",
        4 => "trillion",
        5 => "quadrillion",
        6 => "quintillion",
        7 => "sextillion",
        8 => "septillion",
        9 => "octillion",
        10 => "nonillion",
        11 => "decillion",
        12 => "undecillion",
        13 => "duodecillion",
        14 => "tredecillion",
        _ => unimplemented!(),
    }
}

fn ones(n: u64) -> &'static str {
    match n {
        0 => "",
        1 => "one",
        2 => "two",
        3 => "three",
        4 => "four",
        5 => "five",
        6 => "six",
        7 => "seven",
        8 => "eight",
        9 => "nine",
        _ => unreachable!(),
    }
}

fn tens(n: u64) -> String {
    debug_assert!(n <= 99);

    let digit_ones = n % 10;

    match n {
        0..=9 => String::from(ones(digit_ones)),
        10 => String::from("ten"),
        11 => String::from("eleven"),
        12 => String::from("twelve"),
        13 => String::from("thirteen"),
        14 => String::from("fourteen"),
        15 => String::from("fifteen"),
        16 => String::from("sixteen"),
        17 => String::from("seventeen"),
        18 => String::from("eighteen"),
        19 => String::from("nineteen"),
        20 => String::from("twenty"),
        21..=29 => String::from("twenty-") + ones(digit_ones),
        30 => String::from("thirty"),
        31..=39 => String::from("thirty-") + ones(digit_ones),
        40 => String::from("forty"),
        41..=49 => String::from("forty-") + ones(digit_ones),
        50 => String::from("fifty"),
        51..=59 => String::from("fifty-") + ones(digit_ones),
        60 => String::from("sixty"),
        61..=69 => String::from("sixty-") + ones(digit_ones),
        70 => String::from("seventy"),
        71..=79 => String::from("seventy-") + ones(digit_ones),
        80 => String::from("eighty"),
        81..=89 => String::from("eighty-") + ones(digit_ones),
        90 => String::from("ninety"),
        91..=99 => String::from("ninety-") + ones(digit_ones),
        _ => unreachable!(),
    }
}
