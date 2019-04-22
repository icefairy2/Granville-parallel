granville(A, B, NT, T) :-
    statistics(walltime, _),
    retractall(in_set(_)),
    assert(in_set(1)),
    NT1 is NT - 1,
    create_granville_threads(A, B, NT, NT1, L),
    join_granville_threads(L),
    statistics(walltime, [_|[T]]).

create_granville_threads(A, B, NT, 0, [ID]) :-
    thread_create(start_granville_thread(A, 0, B, NT), ID, []),
    !.

create_granville_threads(A, B, NT, NT1, [H|T]) :-
    NT2 is NT1 - 1,
    create_granville_threads(A, B, NT, NT2, T),
    thread_create(start_granville_thread(A, NT1, B, NT), H, []).

join_granville_threads([]) :- !.

join_granville_threads([H|T]) :-
    join_granville_threads(T),
    thread_join(H, _).

start_granville_thread(A, ID, B, NT) :-
    A1 is A+ID,
    thread_self(X),
    writeln(X),
    granville_thread(A1, B, NT).

granville_thread(B, B, _) :- !.

granville_thread(START, B, STEP) :-
    calculate_granville(START),
    !,
    START1 is START + STEP,
    START1 =< B,
    granville_thread(START1, B, STEP).

calculate_granville(N) :-
    ND is ceiling(N / 2),
    divisor_sum(ND, N, S),
    S < N,
    !,
    assert(in_set(N)).

calculate_granville(N) :-
    ND is ceiling(N / 2) + 1,
    divisor_sum(ND, N, S),
    S = N,
    assert(in_set(N)),
    write('Found '),
    writeln(S).

calculate_granville(N) :-
    ND is ceiling(N / 2) + 1,
    divisor_sum(ND, N, S),
    S > N.

divisor_sum(1, _, 1) :- !.

divisor_sum(ND, N, SR) :-
    0 is N mod ND,
    in_set(ND),
    !,
    N1 is ND -1,
    divisor_sum(N1, N, S),
    SR is S + ND.

divisor_sum(ND, N, S) :-
    N1 is ND -1,
    divisor_sum(N1, N, S).
