pub struct Matrix<'a> {
    matrix: &'a str,
}

impl <'a> Matrix<'a>{
    pub fn new(input: &'a str) -> Self {
        Matrix { matrix: input }
    }

    pub fn row(&self, row_no: usize) -> Option<Vec<u32>> {
        self.matrix.lines()
            .nth(row_no - 1)
            .map(|line| {
                line.split_whitespace()
                    .filter_map(|num| num.parse::<u32>().ok())
                    .collect()
            })
    }

    pub fn column(&self, col_no: usize) -> Option<Vec<u32>> {
        self.matrix.lines()
            .map(|line| {
                line.split_whitespace().nth(col_no - 1)
                    .and_then(|num| num.parse::<u32>().ok())
            }).collect()
    }
}
