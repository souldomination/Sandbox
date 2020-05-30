/*
 * Tri-Diagonal Matrix Algorithm (TDMA) with MPI
 *
 * This program solves 1D Poisson equation u'' = -1 on the region 0 <= x <= 1
 * with the boundary condition u(0) = u(1) = 0.
 *
 * The region is divided equally in n+1 intervals with n internal points and
 * 2 boundary points. Under this discretization scheme, the equation becomes:
 *     u(i-1) - 2u(i) + u(i+1) = -h^2
 * where u(i) is a value of u at i-th point (either internal or boundary) and
 * h = 1 / (n+1) is the length of an interval.
 *
 * The output of this program is the values of u at each internal points.
 */

#include <stdio.h>
#include <mpi.h>

const int n = 16;
const double h = 1. / (n+1);

/* Function for forward elimination */
void forward(double *a, double *b, double *c, double *d, int s, int e) {
    for (int k = s+1; k <= e; k++) {
        double m = a[k] / b[k-1];
        b[k] -= m * c[k-1];
        d[k] -= m * d[k-1];
    }
}

/* Function for back substitution */
void back(double *b, double *c, double *d, double *x, int s, int e) {
    for (int k = e-1; k >= s; k--) {
        x[k] = (d[k] - c[k] * x[k+1]) / b[k];
    }
}

int main(int argc, char **argv) {
    int myid;           /* Processor id (rank) of current process */
    int num_procs;      /* Total number of processes (#procs) */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    /* Basic assumption: n is divisible in #procs */
    if (n % num_procs != 0) {
        if (myid == 0)
            printf("#procs must be a divisor of %d!\n", n);
        MPI_Finalize();
        return 0;
    }

    /* Number of elements that this process will calculate */
    int n_local = n / num_procs;

    /* Local TDMA matrix */
    double a[n_local+2], b[n_local+2], c[n_local+2], d[n_local+2], x[n_local+2];

    /* Fill TDMA matrix */
    for (int i = 1; i <= n_local; i++) {
        a[i] = 1;
        b[i] = -2;
        c[i] = 1;
        d[i] = -h*h;
    }

    /* foward elimination */
    if (myid == 0) {
        forward(a, b, c, d, 1, n_local);
    }
    else {
        double firsts[3];
        MPI_Recv(firsts, 3, MPI_DOUBLE, myid-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        b[0] = firsts[0];
        c[0] = firsts[1];
        d[0] = firsts[2];
        forward(a, b, c, d, 0, n_local);
    }
    if (myid != num_procs-1) {
        double lasts[3] = {b[n_local], c[n_local], d[n_local]};
        MPI_Send(lasts, 3, MPI_DOUBLE, myid+1, 0, MPI_COMM_WORLD);
    }

    /* back substitution */
    if (myid == num_procs-1) {
        x[n_local] = d[n_local] / b[n_local];
        back(b, c, d, x, 1, n_local);
    }
    else {
        MPI_Recv(&x[n_local+1], 1, MPI_DOUBLE, myid+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        back(b, c, d, x, 1, n_local+1);
    }
    if (myid != 0) {
        MPI_Send(&x[1], 1, MPI_DOUBLE, myid-1, 1, MPI_COMM_WORLD);
    }

    /* send and print */
    if (myid == 0) {
        for (int i = 1; i <= n_local; i++)
            printf("%.5lf ", x[i]);
        for (int p = 1; p <= num_procs-1; p++) {
            MPI_Recv(x, n_local+2, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 1; i <= n_local; i++)
                printf("%.5lf ", x[i]);
        }
    }
    else {
        MPI_Send(x, n_local+2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
