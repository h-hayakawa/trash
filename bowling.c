#include<stdio.h>


void print_score_sheet(int* pin_log){
  int i;
  int score[10] = {0};
  int score_sum[10] = {0};
  int sum_tmp = 0;
  int strike = 0;
  int spare = 0;
  int frame = 0;
  int score_fixed = -1;
  /* スコア計算 */
  for (i = 0 ; i < 20 - strike; i ++){
    if (pin_log[i] != -1){
      score[frame] += pin_log[i]; /* 基礎点 */
      
      strike = spare = 0;
      if (i%2 == 0){
        if (pin_log[i] == 10){
          strike = 1;
        }
      } else {
        if (pin_log[i] + pin_log[i - 1] == 10){
          spare = 1;
        }
      }
      if (strike){
        int j, ct = 0;
        for (j = i + 1 ; j < i + 5 && j < 21 && ct < 2; j++){
          if (pin_log[j] != -1){
            score[frame] += pin_log[j]; /* 加算点 */
            ct ++;
          }
        }
        if (ct == 2){
          score_fixed = frame;
        }
      }
      if (spare){
        int j, ct = 0;
        for (j = i + 1 ; j < i + 5 && j < 21 && ct < 1; j++){
          if (pin_log[j] != -1){
            score[frame] += pin_log[j]; /* 加算点 */
            ct ++;
          }
        }
        if (ct == 1){
          score_fixed = frame;
        }
      }
      if (!strike && !spare && i%2){
        score_fixed = frame;
      }
    }
    if (i%2){
      frame ++;
    }
  }
  for (i = 0 ; i < score_fixed + 1; i ++){
    sum_tmp += score[i];
    score_sum[i] = sum_tmp;
  }
  
  /* 以下表示整形 */
  fprintf(stdout, "|");
  for (i = 1 ; i < 10; i ++){
    fprintf(stdout, " %2d|", i);
  }
  fprintf(stdout, "   %2d|\n", i);
  
  /* 倒したピン数表示。最終フレームの場合分けめんどい。 */
  for (i = 0 ; i < 21; i ++){
    if (i%2){
      fprintf(stdout, " ");
    } else {
      if (i == 20){
        fprintf(stdout, " ");
      } else {
        fprintf(stdout, "|");
      }
    }
    if (pin_log[i] == -1){
      fprintf(stdout, " ");
    } else {
      if (pin_log[i] == 10){
        if (i >= 19){
          if (pin_log[i - 1] < 10){
            if (i == 20 && (pin_log[i - 1] + pin_log[i - 2] == 10 && pin_log[i - 1])){
              fprintf(stdout, "X");
            } else {
              fprintf(stdout, "/");
            }
          } else {
            fprintf(stdout, "X");
          }
        } else {
          if (i%2){
            fprintf(stdout, "/");
          } else {
            fprintf(stdout, "X");
          }
        }
      } else if (pin_log[i] == 0){
        if (i%2){
          if (i == 19 && pin_log[18] == 10){
            fprintf(stdout, "G");
          } else {
            fprintf(stdout, "-");
          }
        } else {
          fprintf(stdout, "G");
        }
      } else {
        if (i%2){
          if (pin_log[i] + pin_log[i - 1] == 10){
            fprintf(stdout, "/");
          } else {
            fprintf(stdout, "%d", pin_log[i]);
          }
        } else {
          if (i == 20 && pin_log[i] + pin_log[i - 1] == 10 && !(pin_log[i - 1] + pin_log[i - 2] == 10)){
            fprintf(stdout, "/");
          } else {
            fprintf(stdout, "%d", pin_log[i]);
          }
        }
      }
    }
  }
  fprintf(stdout, "|\n");
  fprintf(stdout, "|");
  for (i = 0 ; i < 10; i ++){
    if (i == 9){
      fprintf(stdout, "  ");
    }
    if (i < score_fixed + 1){
      fprintf(stdout, "%3d|", score_sum[i]);
    } else {
      fprintf(stdout, "   |");
    }
  }
  fprintf(stdout, "\n");
}


int main(){
  char buf[4096];
  int game_over = 0;
  int n_frame = 1; /* 何フレーム目か */
  int nth_throw = 1; /* フレーム内の何投球目か */
  int pin_log[21] = {0}; /* 倒したピン数ログ */ 
  int score[10] = {0};
  int score_sum = 0;
  int strike = 0;
  int spare = 0;
  int remain_pin = 10; /* 残りピン数 */
  int throw_num; /* 投球総数 */
  int i;
  
  /* 倒したピン履歴初期化 */
  for (i = 0 ; i < 21; i ++){
    pin_log[i] = -1;
  }
      
  while (!game_over){
    print_score_sheet(pin_log);
    fprintf(stdout, "第%dフレーム\n", n_frame);
    fprintf(stdout, "第%d投 残ピン%d\n", nth_throw, remain_pin);
    fprintf(stdout, "倒れたピン数を入力してください。\n");

    
    /* 標準入力から1行読み込む */
    if (fgets(buf, sizeof(buf), stdin)){
      if (buf[0] == '\n'){ /* 改行文字だけの入力 */
        fprintf(stdout, "入力エラー:倒れたピン数が不正です。再入力してください。\n");
        continue;
      }
      int n_pin = atoi(buf); /* 倒したピン数 */
      if (remain_pin < n_pin){
        fprintf(stdout, "入力エラー:倒れたピン数が不正です。再入力してください。\n");
        continue;
      }
      if (strike){
        strike --;
      }
      if (spare){
        spare --;
      }
      if (n_pin == 10){ /* ストライク */
        strike = 2;
      } else if (remain_pin - n_pin == 0){ /* スペア */
        spare = 1;
      }
      /* 倒したピン数履歴 */
      int log_idx = (n_frame - 1) * 2 + (nth_throw - 1);
      pin_log[log_idx] = n_pin;
      remain_pin -= n_pin;
    } else {
      fprintf(stdout, "入力エラー:fgets失敗 or EOF\n");
      game_over = 1;
    }
    if (n_frame >= 10){ /* 10フレーム目 */
      if (nth_throw >= 2 && !strike && !spare){ /* 2投目でストライクでもスペアでもない */
        game_over = 1;
        continue;
      }
      if (nth_throw >= 3){ /* 3投目 */
        game_over = 1;
        continue;
      }
      if (remain_pin == 0){ /* 残りピンが無いけどゲーム続行 ピン復活 */
        remain_pin = 10;
      }
    } else { /* 9フレーム目まで */
      if (nth_throw >= 2){ /* 2投目は次のフレームへ */
        remain_pin = 10;
        nth_throw = 1;
        n_frame ++;
        continue;
      }
      if (!remain_pin){ /* ピンが無い場合は次のフレームへ */
        remain_pin = 10;
        nth_throw = 1;
        n_frame ++;
        continue;
      }
      /* ピンがあり1投目 */
    }
    nth_throw ++;
  }
  print_score_sheet(pin_log);
  return 0;
}