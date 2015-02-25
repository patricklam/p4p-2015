void GATSP::iteration()
{
    /* Get our selections */
    selection_container selections = selection();

    /* Create our new population and fill it based on the selections */
    population_container new_population;
    for (auto& selection : selections) {
        auto& individual_a = *selection.first;
        auto& individual_b = *selection.second;
        /* Perform the crossover and add the new individual to the new population */
        new_population.push_back(crossover(individual_a, individual_b));
        /* Sometimes randomly mutate this new individual */
        if (kMutationProbability >= (static_cast<double>(rand()) / static_cast<double>(RAND_MAX))) {
            mutate(new_population.back());
        }
    }

    // ...
}
