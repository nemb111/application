#[derive(Debug)]
pub struct ChessPosition {
    rank: i32,
    file: i32,
}

#[derive(Debug)]
pub struct Queen(ChessPosition);

impl ChessPosition {
    pub fn new(rank: i32, file: i32) -> Option<Self> {
        let valid_range = 0..7;
        if valid_range.contains(&rank) && valid_range.contains(&file) {
            return Some(Self { rank, file });
        }
        None
    }
}

impl Queen {
    pub fn new(position: ChessPosition) -> Self {
        Self(position)
    }

    pub fn can_attack(&self, other: &Queen) -> bool {
        let ChessPosition {
            rank: q1_rank,
            file: q1_file,
        } = self.0;
        let ChessPosition {
            rank: q2_rank,
            file: q2_file,
        } = other.0;

        if q1_rank == q2_rank || q1_file == q2_file {
            true
        } else {
            (q1_rank - q2_rank).abs() == (q1_file - q2_file).abs()
        }
    }
}
