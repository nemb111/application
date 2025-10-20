const N_ROWS: usize = 4;
const N_COLS: usize = 3;

#[rustfmt::skip]
const OCD_NUMS : &[& str] = &[
    concat!(
        " _ ",
        "| |",
        "|_|",
        "   "
    ),
    concat!(
        "   ",
        "  |",
        "  |",
        "   "
    ),
    concat!(
        " _ ",
        " _|",
        "|_ ",
        "   "
    ),
    concat!(
        " _ ",
        " _|",
        " _|",
        "   "
    ),
    concat!(
        "   ",
        "|_|",
        "  |",
        "   "
    ),
    concat!(
        " _ ",
        "|_ ",
        " _|",
        "   "
    ),
    concat!(
        " _ ",
        "|_ ",
        "|_|",
        "   "
    ),
    concat!(
        " _ ",
        "  |",
        "  |",
        "   "
    ),
    concat!(
        " _ ",
        "|_|",
        "|_|",
        "   "
    ),
    concat!(
        " _ ",
        "|_|",
        " _|",
        "   "
    ),
];

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    InvalidRowCount(usize),
    InvalidColumnCount(usize),
}

fn parse_row(row: &[&str]) -> String {
    assert!(row.len() == N_ROWS);

    let mut res = String::new();

    let row_len = row[0].len();
    for i in (0..row_len).step_by(N_COLS) {
        let concated_str = row.iter().flat_map(|s| &s.as_bytes()[i..i + N_COLS]);

        res.push('?');
        OCD_NUMS.iter().enumerate().for_each(|(i, s)| {
            if s.as_bytes().iter().eq(concated_str.clone()) {
                res.pop();
                res.push_str(&i.to_string());
            }
        });
    }

    res
}

pub fn convert(input: &str) -> Result<String, Error> {
    let input_vec: Vec<_> = input
        .lines()
        .map(|line| {
            if line.len() % N_COLS != 0 {
                return Err(Error::InvalidColumnCount(line.len()));
            }

            Ok(line)
        })
        .collect::<Result<Vec<&str>, Error>>()?;

    if input_vec.len() % N_ROWS != 0 {
        return Err(Error::InvalidRowCount(input_vec.len()));
    }

    let mut res = String::new();
    for i in (0..input_vec.len()).step_by(N_ROWS) {
        let row = &input_vec[i..i + N_ROWS];
        res.push_str(&parse_row(row));
        res.push(',');
    }
    res.pop(); // Remove the last comma

    Ok(res)
}
