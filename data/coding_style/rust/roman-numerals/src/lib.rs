use std::fmt::{Display, Formatter, Result};

pub struct Roman {
    roman_numeral: String,
}

impl Display for Roman {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result {
        write!(f, "{}", self.roman_numeral)
    }
}

impl From<u32> for Roman {
    fn from(mut num: u32) -> Self {
        let mut roman_numeral = String::new();
        let mut n = 10;

        while num > 0 {
            let tmp = (num % 10) * (n / 10);
            let mut tmp_str = num_to_roman_numeral(tmp).to_string();
            tmp_str.push_str(&roman_numeral);
            roman_numeral = tmp_str;
            num /= 10;
            n *= 10;
        }

        Self { roman_numeral }
    }
}

fn num_to_roman_numeral<'a>(num: u32) -> &'a str {
    match num {
        0 => "",
        1 => "I",
        2 => "II",
        3 => "III",
        4 => "IV",
        5 => "V",
        6 => "VI",
        7 => "VII",
        8 => "VIII",
        9 => "IX",
        10 => "X",
        20 => "XX",
        30 => "XXX",
        40 => "XL",
        50 => "L",
        60 => "LX",
        70 => "LXX",
        80 => "LXXX",
        90 => "XC",
        100 => "C",
        200 => "CC",
        300 => "CCC",
        400 => "CD",
        500 => "D",
        600 => "DC",
        700 => "DCC",
        800 => "DCCC",
        900 => "CM",
        1000 => "M",
        2000 => "MM",
        3000 => "MMM",
        _ => unreachable!(),
    }
}
