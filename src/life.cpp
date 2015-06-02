#include <gecode/driver.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

class Life: public Script {
private:
	const int n;
	BoolVarArray gridVector;
public:
	Life(const SizeOptions& opt) :
			Script(opt), n(opt.size()), gridVector(*this, (n + 4) * (n + 4), 0, 1) {

		Matrix<BoolVarArgs> grid(gridVector, n + 4, n + 4);

		// Borders should be zero
		rel(*this, grid.row(0), IRT_EQ, 0);
		rel(*this, grid.row(1), IRT_EQ, 0);
		rel(*this, grid.row(n + 2), IRT_EQ, 0);
		rel(*this, grid.row(n + 3), IRT_EQ, 0);

		rel(*this, grid.col(0), IRT_EQ, 0);
		rel(*this, grid.col(1), IRT_EQ, 0);
		rel(*this, grid.col(n + 2), IRT_EQ, 0);
		rel(*this, grid.col(n + 3), IRT_EQ, 0);

		for (int i = 1; i < n + 3; i++) {
			for (int j = 1; j < n + 3; j++) {
				BoolVarArray neighbors(*this, 8, 0, 1);

				// Up
				neighbors[0] = grid(i, j - 1);
				// Down
				neighbors[1] = grid(i, j + 1);
				// Left
				neighbors[2] = grid(i - 1, j);
				// Right
				neighbors[3] = grid(i + 1, j);

				// Up right
				neighbors[4] = grid(i + 1, j - 1);
				// Up left
				neighbors[5] = grid(i - 1, j - 1);
				// Down right
				neighbors[6] = grid(i + 1, j + 1);
				// Down left
				neighbors[7] = grid(i - 1, j + 1);

				IntVar alive(*this, 0, 8);

				rel(*this, sum(neighbors) == alive);

				// Alive cells should remain alive
				rel(*this, grid(i, j) >> (alive >= 2));
				rel(*this, grid(i, j) >> (alive <= 3));

				// Dead cells should remain dead
				rel(*this, !grid(i, j) >> (alive != 3));
			}
		}

		branch(*this, gridVector, INT_VAR_SIZE_MAX(), INT_VAL_MAX());

	}

	// Constructor for cloning
	Life(bool share, Life& life, int size) :
			Script(share, life), n(size) {
		gridVector.update(*this, share, life.gridVector);
	}

	/// Perform copying during cloning
	virtual Space*
	copy(bool share) {
		return new Life(share, *this, n);
	}

	virtual void print(std::ostream& os) const {
		int counter = 0;

		for (int i = 1; i <= gridVector.size(); i++) {
			std::cout << gridVector[i - 1] << " ";
			if (gridVector[i -1].val() == 1)
				counter++;
			if ((i % (n + 4)) == 0) {
				std::cout << std::endl;
			}
		}

		std::cout << "Sum: " << counter << std::endl;

		std::cout << std::endl;
	}

	virtual void constrain(const Space& _home) {
		const Life& home = static_cast<const Life&>(_home);
		rel(*this, sum(gridVector) > sum(home.gridVector));
	}
};

int main(int argc, char* argv[]) {
	SizeOptions opt("Game of Life");
	opt.size(8);
	opt.solutions(0);
	opt.parse(argc, argv);
	Script::run<Life, BAB, SizeOptions>(opt);
	return 0;
}
