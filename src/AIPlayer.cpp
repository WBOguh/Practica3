#include "AIPlayer.h"
#include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move()
{
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color &c_piece, int &id_piece, int &dice) const
{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    // como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    // respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor;                           // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
                                            // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
                                            // valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
                                            // cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch (id)
    {
    case 0:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
        break;
    case 1:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, heuristicav1);
        break;
    case 2:
        // valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
        break;
    }
    /*
        switch (id)
        {
        case 0:
            thinkAleatorio(c_piece, id_piece, dice);
            break;
        case 1:
            thinkAleatorioMasInteligente(c_piece, id_piece, dice);
            break;
        case 2:
            thinkFichaMasAdelantada(c_piece, id_piece, dice);
            break;
        case 3:
            thinkMejorOpcion(c_piece, id_piece, dice);
            break;
        }*/
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
}

double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}
void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const
{
    // El id de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
}

void AIPlayer::thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const
{
    int player = actual->getCurrentPlayerId();
    vector<int> current_dices;
    vector<tuple<color, int>> current_pieces;
    current_dices = actual->getAvailableNormalDices(player);
    vector<int> current_dices_que_pueden_mover_ficha;

    for (int i = 0; i < current_dices.size(); ++i)
    {
        current_pieces = actual->getAvailablePieces(player, current_dices[i]);
        if (current_pieces.size() > 0)
        {
            current_dices_que_pueden_mover_ficha.push_back(current_dices[i]);
        }
    }

    if (current_dices_que_pueden_mover_ficha.size() == 0)
    {
        dice = current_dices[rand() % current_dices.size()];
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor();
    }
    else
    {
        dice = current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];
        current_pieces = actual->getAvailablePieces(player, dice);
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]);
        c_piece = get<0>(current_pieces[random_id]);
    }
}

void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const
{
    int player = actual->getCurrentPlayerId();
    vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);
    int id_ficha_mas_adelantada = -1;
    color col_ficha_mas_adelantada = none;
    int min_distancia_meta = 9999;
    for (int i = 0; i < current_pieces.size(); ++i)
    {
        color col = get<0>(current_pieces[i]);
        int id = get<1>(current_pieces[i]);
        int distancia_meta = actual->distanceToGoal(col, id);
        if (distancia_meta < min_distancia_meta)
        {
            min_distancia_meta = distancia_meta;
            id_ficha_mas_adelantada = id;
            col_ficha_mas_adelantada = col;
        }
    }

    if (id_ficha_mas_adelantada == -1)
    {
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor();
    }
    else
    {
        id_piece = id_ficha_mas_adelantada;
        c_piece = col_ficha_mas_adelantada;
    }
}
void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const
{
    ParchisBros hijos = actual->getChildren();
    bool me_quedo_con_esta_accion = false;
    int current_power = actual->getPowerBar(this->jugador).getPower();
    int max_power = -101;

    for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end() and !me_quedo_con_esta_accion; ++it)
    {
        Parchis siguiente_hijo = *it;
        if (siguiente_hijo.isEatingMove() or siguiente_hijo.isGoalMove() or (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador))
        {
            me_quedo_con_esta_accion = true;
            c_piece = it.getMovedColor();
            id_piece = it.getMovedPieceId();
            dice = it.getMovedDiceValue();
        }
        else
        {
            int new_power = siguiente_hijo.getPower(this->jugador);
            if (new_power - current_power > max_power)
            {
                c_piece = it.getMovedColor();
                id_piece = it.getMovedPieceId();
                dice = it.getMovedDiceValue();
                max_power = new_power - current_power;
            }
        }
    }
}

double AIPlayer::heuristicav1(const Parchis &estado, int jugador)
{
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador += 7;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == home)
                {
                    puntuacion_jugador -= 20;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue)
                {
                    puntuacion_jugador += 10;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 100;
                }
                puntuacion_jugador += 10 - estado.distanceToGoal(c, j);
            }
        }
        if (estado.getPower(jugador) >= 0 and estado.getPower(jugador) < 50)
            puntuacion_jugador += 7 + (estado.getPower(jugador) / 7);
        else if ((estado.getPower(jugador) >= 50 and estado.getPower(jugador) < 60) or (estado.getPower(jugador) >= 70 and estado.getPower(jugador) < 75))
            puntuacion_jugador += 25;
        else if (estado.getPower(jugador) >= 60 and estado.getPower(jugador) < 65)
            puntuacion_jugador -= 1000;
        else if (estado.getPower(jugador) >= 65 and estado.getPower(jugador) < 70)
            puntuacion_jugador += 15;
        else if (estado.getPower(jugador) >= 80 and estado.getPower(jugador) < 85)
            puntuacion_jugador -= 2000;
        else if (estado.getPower(jugador) >= 85 and estado.getPower(jugador) < 90)
            puntuacion_jugador += 30;
        else if (estado.getPower(jugador) >= 90 and estado.getPower(jugador) < 95)
            // puntuacion_jugador += 40;
            puntuacion_jugador -= 4000;
        else if (estado.getPower(jugador) >= 95 and estado.getPower(jugador) < 100)
            puntuacion_jugador += 40;
        else
            puntuacion_jugador -= 5000;

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente += 7;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == home)
                {
                    puntuacion_oponente -= 20;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue)
                {
                    puntuacion_oponente += 10;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 100;
                }
                puntuacion_oponente += 100 - estado.distanceToGoal(c, j);
            }
        }
        if (estado.getPower(oponente) >= 0 and estado.getPower(oponente) < 50)
            puntuacion_oponente += 7 + (estado.getPower(jugador) / 7);
        else if ((estado.getPower(oponente) >= 50 and estado.getPower(oponente) < 60) or (estado.getPower(jugador) >= 70 and estado.getPower(jugador) < 75))
            puntuacion_oponente += 25;
        else if (estado.getPower(oponente) >= 60 and estado.getPower(oponente) < 65)
            puntuacion_oponente -= 1000;
        else if (estado.getPower(oponente) >= 65 and estado.getPower(oponente) < 70)
            puntuacion_oponente += 15;
        else if (estado.getPower(oponente) >= 80 and estado.getPower(oponente) < 85)
            puntuacion_oponente -= 2000;
        else if (estado.getPower(oponente) >= 85 and estado.getPower(oponente) < 90)
            puntuacion_oponente += 30;
        else if (estado.getPower(oponente) >= 90 and estado.getPower(oponente) < 95)
            // puntuacion_oponente += 40;
            puntuacion_oponente -= 4000;
        else if (estado.getPower(oponente) >= 95 and estado.getPower(oponente) < 100)
            puntuacion_oponente += 40;
        else
            puntuacion_oponente -= 5000;

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}
double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidadMax, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristica)(const Parchis &, int)) const
{
    if (profundidad == profundidadMax or actual.gameOver())
    {
        return heuristica(actual, jugador);
    }
    else
    {
        ParchisBros hijos = actual.getChildren();
        if (jugador == actual.getCurrentPlayerId())
        { // Generamos hijos max
            for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it)
            {
                double better_alpha = Poda_AlfaBeta(*it, jugador, profundidad + 1, profundidadMax, c_piece, id_piece, dice, alpha, beta, heuristica);
                if (better_alpha > alpha)
                {
                    alpha = better_alpha;
                    if (profundidad == 0)
                    {
                        c_piece = it.getMovedColor();
                        id_piece = it.getMovedPieceId();
                        dice = it.getMovedDiceValue();
                    }
                }
                if (alpha >= beta)
                {
                    // return beta;
                    break;
                }
            }
            // return alpha;
        }
        else
        { // Generamos hijos min
            for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it)
            {
                double better_beta = Poda_AlfaBeta(*it, jugador, profundidad + 1, profundidadMax, c_piece, id_piece, dice, alpha, beta, heuristica);
                if (better_beta < beta)
                    beta = better_beta;
                if (alpha >= beta)
                {
                    // return alpha;
                    break;
                }
            }
            // return beta;
        }
        if (jugador == actual.getCurrentPlayerId())
        {
            return alpha;
        }
        else
        {
            return beta;
        }
    }
}