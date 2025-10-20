// The code below is a stub. Just enough to satisfy the compiler.
// In order to pass the tests you can add-to or change any of this code.

#[derive(PartialEq, Debug)]
pub enum Direction {
    North,
    East,
    South,
    West,
}

pub struct Robot {
    d: Direction,
    x: i32,
    y: i32,
}

impl Robot {
    pub fn new(x: i32, y: i32, d: Direction) -> Self {
        Self { d, x, y }
    }

    pub fn turn_right(self) -> Self {
        let new_directon: Direction;

        match self.d {
            Direction::North => new_directon = Direction::East,
            Direction::East => new_directon = Direction::South,
            Direction::South => new_directon = Direction::West,
            Direction::West => new_directon = Direction::North,
        }

        Robot {
            d: new_directon,
            ..self
        }
    }

    pub fn turn_left(self) -> Self {
        let new_directon: Direction;

        match self.d {
            Direction::North => new_directon = Direction::West,
            Direction::East => new_directon = Direction::North,
            Direction::South => new_directon = Direction::East,
            Direction::West => new_directon = Direction::South,
        }

        Robot {
            d: new_directon,
            ..self
        }
    }

    pub fn advance(self) -> Self {
        let mut new_x = self.x;
        let mut new_y = self.y;

        match self.d {
            Direction::North => new_y += 1,
            Direction::East => new_x += 1,
            Direction::South => new_y -= 1,
            Direction::West => new_x -= 1,
        }

        Robot {
            d: self.d,
            x: new_x,
            y: new_y,
        }
    }

    pub fn instructions(self, instructions: &str) -> Self {
        let mut robot = self;

        for c in instructions.chars() {
            match c {
                'L' => robot = robot.turn_left(),
                'R' => robot = robot.turn_right(),
                'A' => robot = robot.advance(),
                _ => (),
            }
        }

        robot
    }

    pub fn position(&self) -> (i32, i32) {
        (self.x, self.y)
    }

    pub fn direction(&self) -> &Direction {
        &self.d
    }
}
