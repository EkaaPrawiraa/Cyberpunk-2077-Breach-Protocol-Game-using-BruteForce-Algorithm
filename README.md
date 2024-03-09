\algrenewcomment[1]{\textsf{\hfill//#1}}
\begin{algorithmic}[1]\ttfamily
    \State from typing import Optional
    \State from game.logic.base import BaseLogic
    \State from game.models import Board,GameObject,Position,Config
    \State from typing import Optional
    \State import random
    \State import time
    \State
    \State //Tubes Stima 1 Point :
    \State //1. Utamain diamond terdekat dan diamond merah kalo bisa
    \State //2. Kalo ada base terdekat dan jauh dari diamond dan inventory tidak kosong balik ke base
    \State //3. menjauh dari pemain lawan atau ambil inventorynya
    \State //4. optimalin teleport dan red button
    \State
    \State //Developing note :
    \State //Cari diamond banyak yang deket sama base aja, rata rata max point yang didapet itu cuman 10
    \State //error pas inevntory == 4 terus dapet red diamond
    \State //fungsi buat calculate jarak ke tiap goal point
    \State //masih banyak keliling gajelas perlu kalkulasi lama jalan sama waktu sisa biar optimal
    \State //buat metode untuk atur prioritas untuk balik ke base dulu atau tetep jalan ngambilin diamond
    \State //tiap satu langkah kira kira makan 1 detik
    \State //ukuran table masih fix 15*15 padahal ganentu
    \State //belum optimal fitur red button sama teeport
    \State //buat fungsi menghindari teleport agar tidak looping
    \State //kalo diamond abis, auto ke generate diamond, pilihannya klo sisa 1 diamond, pencet red button atau ambil diamond
    \State
    \State
    \State \# \Comment{\ algortima nextmove}
    \State class BotsMove(BaseLogic)
        \Function{__init__}{self}
        \State //Intialize attribute necessary
                \State self.directions $\leftarrow$ [(1, 0), (0, 1), (-1, 0), (0, -1)]
                \State self.block_in $\leftarrow$ [(0,0),(0,1),(0,2),(1,0),(1,1),(1,2),(2,0),(2,1),(2,2)]
                \State self.goal_position: Optional[Position] $\leftarrow$ None
                \State //pengganti fungsi clam
                \EndFunction
        \Function{boundary}{self, n, smallest, largest}
            \State\Return max(smallest, min(n, largest))
            \State //pengganti fungsi get direction sesuai kriteria baru
            \EndFunction
        \Function{get_way}{self, current_x, current_y, dest_x, dest_y, base, telestart:Position, teletarget:Position, checktele}
            \State delta_x $\leftarrow$ self.boundary(dest_x - current_x, -1, 1)
            \State delta_y $\leftarrow$ self.boundary(dest_y - current_y, -1, 1)
            \State check $\leftarrow$ False
            \State
            \If{$\lnot$ checktele}
            \State //Periksa posisi berikutnya
                \State next_pos_x $\leftarrow$ current_x + delta_x
                \State next_pos_y $\leftarrow$ current_y + delta_y
                \State //periksa jika melewati teleport
                \If{(next_pos_x = telestart.x $ \land $ next_pos_y = telestart.y)}
                \State //Perhitungkan berdasarkan posisi terhadap base
                    \State if(next_pos_x = telestart.x $ \land $ current_y+1 = telestart.y) $ \land $ (next_pos_y $\neq$ telestart.x)
                        \State delta_y $\leftarrow$ 0
                    \Else 
                        \State delta_x $\leftarrow$ 0
                        \EndIf
                    \State check $\leftarrow$ True
                    \State
                    \State
                    \EndIf
                \State elif(next_pos_x = teletarget.x $ \land $ next_pos_y = teletarget.y)
                    \State if(next_pos_x = teletarget.x $ \land $ current_y+1 = teletarget.y) $ \land $ (next_pos_y $\neq$ teletarget.x)
                        \State delta_y $\leftarrow$ 0
                    \Else 
                        \State delta_x $\leftarrow$ 0
                        \EndFunction
                    \State check $\leftarrow$ True
                    \State
            \If{(delta_x=0 $ \land $ delta_y=0)} \Comment{\ buat tele}
                \If{(current_x = 0 $\lor$ current_y = 0)}
                    \State delta_x $\leftarrow$ 1
                \ElsIf{(current_x = 14 $\lor$ current_y = 14)}
                    \State delta_x $\leftarrow$ -1
                \Else 
                    \State delta_x $\leftarrow$ 1
                    \State delta_y $\leftarrow$ 0
                \ElsIf{(not check)}
                    \EndIf
                \If{delta_x $\neq$ 0}
                    \State delta_y $\leftarrow$ 0
                    \State
                    \State
                    \EndIf
                \EndIf
            \State\Return (delta_x, delta_y)
            \State
            \State //fungsi untuk mengejar bot lain jika berada di dekat  bot kita
        \Function{chase}{self, bot2:GameObject}
            \State self.goal_position $\leftarrow$ bot2.position
            \State //fungsi untuk mencari apakah ada bot lain di board
            \EndFunction
        \Function{get_tacklebot}{self, board:Board, board_bot}
            \State item $\leftarrow$ board.game_objects
            \State tacklers $\leftarrow$ []
            \For{a $\in$ item}
            \State //bot lain memiliki type BotGameObject dan pastikan itu bukan bot kita
                \If{a.type="BotGameObject" $ \land $ a.properties.can_tackle $ \land $ a.position$\neq$board_bot.position}
                    \State tacklers.append(a)
                    \EndIf
                \EndFor
            \State\Return tacklers
            \State //fungsi untuk mencari lokasi redbutton dan juga teleport
            \EndFunction
        \Function{get_redbut_telep}{self, board,board_bot}
            \State item_board $\leftarrow$ board.game_objects
            \State teleports $\leftarrow$ []
            \State current_position $\leftarrow$ board_bot.position
            \State checkred $\leftarrow$ False
            \State checktele $\leftarrow$ False
            \For{item $\in$ item_board}
            \State //kasus berhenti ketika lokasi dua duanya telah ditemukan
                \State if(checkred) $ \land $ (checktele)
                    \State break
                \Else 
                    \State //Mencari dengan kondisi sesuai typenya
                    \If{item.type='DiamondButtonGameObject'}
                        \State red $\leftarrow$ item
                        \State checkred $\leftarrow$ True
                    \ElsIf{item.type='TeleportGameObject'}
                        \State teleports.append((abs(abs(item.position.x - current_position.x) + abs(item.position.y - current_position.y)), item.position))
                        \State if(len(teleports)$>$1)
                            \State sorted_teleport $\leftarrow$ sorted(teleports,key $\leftarrow$ lambda x: x[0])
                            \State checktele $\leftarrow$ True
                            \State
                            \EndIf
                        \EndFor
                    \EndFunction
            \State redpos $\leftarrow$ red.position
            \State
            \State\Return redpos, sorted_teleport
            \State //fungsi untuk mencari total point diamond pada block bot berada
        \Function{current_totalpointblock}{self, current_position, diamond_objects}
        \State //block bot didapatkan dengan membagi total board menjai 9 block
            \State whichBlock_x $\leftarrow$ current_position.x//5
            \State whichBlock_y $\leftarrow$ current_position.y//5
            \State block_start_row $\leftarrow$ whichBlock_x * 5
            \State block_end_row $\leftarrow$ (whichBlock_x + 1) * 5
            \State block_start_col $\leftarrow$ whichBlock_y * 5
            \State block_end_col $\leftarrow$ (whichBlock_y + 1) * 5
            \State totalpointblock $\leftarrow$ 0
            \State listrangediamond $\leftarrow$ []
            \For{k $\in$ range(block_start_row,block_end_row)}
                    \For{l $\in$ range(block_start_col,block_end_col)}
                        \For{diamond $\in$ diamond_objects}
                            \If{(diamond.position.x=k ) $ \land $ (diamond.position.y=l)}
                                \State totalpointblock+ $\leftarrow$ diamond.properties.points
                                \State listrangediamond.append((abs(abs(diamond.position.x - current_position.x) + abs(diamond.position.y - current_position.y)), diamond.position, diamond.properties.points))
                                \EndIf
                            \EndFor
                        \EndFor
                    \EndFor
                \EndFunction
            \State\Return totalpointblock, listrangediamond
            \State //fungsi untuk mencari point total block pada semua block dan mencari lokasi terdekat dengan base bot kita
        \Function{totalpointblock}{self,start_position,diamond_objects}
        \State
            \State totaleveryblock $\leftarrow$ []
            \For{i $\in$ range(3)}
                \For{j $\in$ range(3)}
                        \State block_start_row $\leftarrow$ 5 * i
                        \State block_end_row $\leftarrow$ (1 + i) * 5
                        \State block_start_col $\leftarrow$ j * 5
                        \State block_end_col $\leftarrow$ (1 + j) * 5
                        \State totalpointblock $\leftarrow$ 0
                        \State listrangediamond $\leftarrow$ []
                        \State //looping untuk tiap block
                        \For{k $\in$ range(block_start_row,block_end_row)}
                                \For{l $\in$ range(block_start_col,block_end_col)}
                                    \For{diamond $\in$ diamond_objects}
                                        \If{(diamond.position.x=k ) $ \land $ (diamond.position.y=l)}
                                            \State totalpointblock+ $\leftarrow$ diamond.properties.points
                                            \State listrangediamond.append((abs(abs(diamond.position.x - start_position.x) + abs(diamond.position.y - start_position.y)), diamond.position))
                                            \EndIf
                                        \EndFor
                                    \EndFor
                                \EndFor
                            \EndFor
                        \State sorted_listrangediamond $\leftarrow$ sorted(listrangediamond, key $\leftarrow$ lambda x: x[0]) \Comment{\ sort berdasar jarak start position}
                        \If{sorted_listrangediamond}
                            \State distance,locationdiamond $\leftarrow$ sorted_listrangediamond[0]
                            \State totaleveryblock.append((totalpointblock,distance,locationdiamond))
                            \EndIf
                        \EndFor
                    \EndFunction
            \State sorted_totaleveryblock $\leftarrow$ sorted(totaleveryblock,key $\leftarrow$ lambda x: x[1])
            \State
            \State _,_, self.goal_position $\leftarrow$ sorted_totaleveryblock[0]
            \State\Return self.goal_position
            \State
            \State
        \Function{next_move}{self, board_bot: GameObject, board: Board}
                \State props $\leftarrow$ board_bot.properties
                \State //Analyze new state
                \State base $\leftarrow$ board_bot.properties.base
                \State current_position $\leftarrow$ board_bot.position
                \State distanceToBase $\leftarrow$ abs(abs(base.x- current_position.x) + abs(base.y - current_position.y))
                \State redpos, teleport $\leftarrow$ self.get_redbut_telep(board,board_bot)
                \State _, telestart $\leftarrow$ teleport[0]
                \State _, teletarget $\leftarrow$ teleport[1]
                \State checktele $\leftarrow$ False
                \If{props.diamonds = 5 $\lor$ props.diamonds = 4  $\lor$ ( (distanceToBase +1 = (props.milliseconds_left/1000)) $ \land $ props.diamonds $\neq$0) $\lor$ (distanceToBase = (props.milliseconds_left/1000))} \Comment{\ error pas inventory diamond = 4 terus dapet diamond merah}
                \State //Move to base
                \State
                    \State base $\leftarrow$ board_bot.properties.base
                    \State self.goal_position $\leftarrow$ base
                    \State bots $\leftarrow$ self.get_tacklebot(board, board_bot)
                    \State //check selama perjalanan ke base ada bot lain dan mengindar
                    \For{bot $\in$ bots}
                        \If{(bot.position.x//5 = current_position.x//5) $ \land $ bot.position.y//5=current_position.y//5}
                            \State delx $\leftarrow$ self.goal_position.x-bot.position.x
                            \State dely $\leftarrow$ self.goal_position.y-bot.position.y
                            \State self.goal_position.x+ $\leftarrow$ delx
                            \State self.goal_position.y+ $\leftarrow$ dely
                            \If{self.goal_position.x$>$14}
                                \State self.goal_position.x $\leftarrow$ 14
                                \EndIf
                            \If{self.goal_position.x$<$0}
                                \State self.goal_position.x $\leftarrow$ 0
                                \EndIf
                            \If{self.goal_position.y$>$14}
                                \State self.goal_position.y $\leftarrow$ 14
                                \EndIf
                            \If{self.goal_position.y$<$0}
                                \State self.goal_position.y $\leftarrow$ 0
                                \EndIf
                            \State break
                        \Else 
                            \EndIf
                        \EndFor
                    \State diamond_objects $\leftarrow$ board.diamonds
                    \State totalpointblock, listrangediamond $\leftarrow$ self.current_totalpointblock(current_position, diamond_objects)
                    \State total_teletarget,_ $\leftarrow$ self.current_totalpointblock(teletarget, diamond_objects)
                    \State //kondisi jika total point di block kita 0
                    \If{totalpointblock=0: \#} \Comment{\ bagian red buttoon}
                    \State
                    \State //kondisi kalo ada red button terdekat
                        \If{((redpos.x//5 = current_position.x//5) $ \land $ (redpos.y//5 = current_position.y//5))}
                            \State self.goal_position $\leftarrow$ redpos
                            \State //kondisi jika ada teleport terdekat dan point teleport tujuannya banyak
                        \ElsIf{((telestart.x//5 = current_position.x//5) $ \land $ (telestart.y//5 = current_position.y//5)) $ \land $ (total_teletarget $\neq$ 0)}
                            \State checktele $\leftarrow$ True
                            \State self.goal_position $\leftarrow$ telestart
                            \State //kondisi mencari ke block lain
                        \Else 
                            \State self.goal_position $\leftarrow$ self.totalpointblock(current_position, diamond_objects)
                            \State //jika ingin chase(tidak efektif)
                            \State //bots=self.get_tacklebot(board, board_bot)
                            \State //for bot in bots:
                            \State //if bot.position.x in range(current_position.x -2, current_position.x +3):
                            \State //if bot.position.y in range(current_position.x -2, current_position.x +3):
                            \State //self.chase(bot)
                            \State //break
                            \State
                            \State
                            \State //kondisi kalo current block point tidak 0 dan mencari diamond terdekat
                        \Else 
                            \EndIf
                        \State sorted_listrangediamond $\leftarrow$ sorted(listrangediamond, key $\leftarrow$ lambda x: x[0])
                        \State //mengatasi error ketika diamond kita 4 dan ada red diamond
                        \If{props.diamonds=4 $ \land $ sorted_listrangediamond[0][2]=2}
                            \State sorted_listrangediamond.pop(0)
                            \EndIf
                        \If{len(sorted_listrangediamond)=0}
                            \State self.goal_position $\leftarrow$ base
                        \Else 
                            \State _,self.goal_position,_ $\leftarrow$ sorted_listrangediamond[0]
                            \State
                            \State
                            \EndIf
                        \EndIf
                    \EndIf
                \State delta_x, delta_y $\leftarrow$ self.get_way(
                    \State current_position.x,
                    \State current_position.y,
                    \State self.goal_position.x,
                    \State self.goal_position.y,
                    \State base,
                    \State telestart,
                    \State teletarget,
                    \State checktele,
                    \EndFunction
                \State )
                \State
                \State\Return delta_x,delta_y
\end{algorithmic}
