granville(A, B, NT, Time) :-
    retractall(in_set(X)),
    assert(in_set(1)),
    granville_threads(A, B, NT),
    statistics(runtime, [Time|_]).

create_granville_threads(A, B, NT) :-
     NT1 is NT - 2,
     forall(
        between(0, NT1, X),
        X1 is A+X,
        thread_create(granville_thread(X1, B, NT),_, [])
    ).

granville_thread(START, B, STEP) :-
    calculate_granville(START),
    START1 is START + STEP,
    START1 =< B,
    granville_thread(START1, B, STEP).

calcualte_granville(N) :-
    divisor_sum(N, S),
    S < N,
    assert(in_set(S)).

calcualte_granville(N) :-
    divisor_sum(N, S),
    S = N,
    assert(in_set(S)),
    write('Found '),
    write(S).

divisor(1, 1).

divisor_sum(N, S) :-
