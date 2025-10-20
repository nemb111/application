use std::convert::TryInto;

pub fn spiral_matrix(size: u32) -> Vec<Vec<u32>> {
    let spiral = Spiral::new(size);
    spiral.get()
}

struct Spiral {
    matrix: Vec<Vec<u32>>,
    direction: (i8, i8),
}

impl Spiral {
    pub fn new(size: u32) -> Self {
        let matrix = vec![vec![u32::from(b'?'); size as usize]; size as usize];
        let direction = (1, 0);

        Self { matrix, direction }
    }

    pub fn get(mut self) -> Vec<Vec<u32>> {
        let (mut x, mut y): (usize, usize) = (0, 0);

        for i in 1..=self.matrix.len().pow(2) {
            self.matrix[y][x] = i.try_into().unwrap();
            if self.needs_rotate((x, y)) {
                self.rotate();
            }
            let (next_x, next_y) = self.get_next_indices((x, y));
            x = next_x;
            y = next_y;
        }

        self.matrix
    }

    fn needs_rotate(&self, (x, y): (usize, usize)) -> bool {
        let (next_x, next_y) = self.get_next_indices((x, y));

        if next_x >= self.matrix.len()
            || next_y >= self.matrix.len()
            || self.matrix[next_y][next_x] != b'?'.into()
            || x as isize + (self.direction.0 as isize) < 0
            || y as isize + (self.direction.1 as isize) < 0
        {
            return true;
        }

        false
    }

    fn rotate(&mut self) {
        match self.direction {
            (1, 0) => self.direction = (0, 1),
            (0, 1) => self.direction = (-1, 0),
            (-1, 0) => self.direction = (0, -1),
            (0, -1) => self.direction = (1, 0),
            _ => unreachable!(),
        }
    }

    fn get_next_indices(&self, (x, y): (usize, usize)) -> (usize, usize) {
        (
            (x as isize + self.direction.0 as isize) as usize,
            (y as isize + self.direction.1 as isize) as usize,
        )
    }
}
