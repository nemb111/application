pub struct PascalsTriangle {
    row_count: u32,
}

impl PascalsTriangle {
    pub fn new(row_count: u32) -> Self {
        Self { row_count }
    }

    pub fn rows(&self) -> Vec<Vec<u32>> {
        let mut rows_vec: Vec<Vec<u32>> = Vec::with_capacity(self.row_count as usize);

        if 0 == self.row_count {
            return vec![];
        }
        rows_vec.push(vec![1]);

        if 1 == self.row_count {
            return rows_vec;
        }
        rows_vec.push(vec![1, 1]);

        if 2 == self.row_count {
            return rows_vec;
        }

        for i in 3..=self.row_count {
            let mut row_vec = Vec::with_capacity(i as usize);

            row_vec.push(1);
            for j in 1..=i - 2 {
                let prev_row = &rows_vec[(i - 2) as usize];
                row_vec.push(prev_row[(j - 1) as usize] + prev_row[j as usize]);
            }
            row_vec.push(1);
            rows_vec.push(row_vec);
        }

        rows_vec
    }
}
