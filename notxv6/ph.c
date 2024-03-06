#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>

#define NBUCKET 5
#define NKEYS 100000

struct entry {
  int key;
  int value;
  struct entry *next;
};
struct entry *table[NBUCKET];
int keys[NKEYS];
int nthread = 1;

pthread_mutex_t bucket_lk[NBUCKET];
double
now()
{
 struct timeval tv;
 gettimeofday(&tv, 0);
 return tv.tv_sec + tv.tv_usec / 1000000.0;
}

static void 
insert(int key, int value, struct entry **p, struct entry *n)
{
  struct entry *e = malloc(sizeof(struct entry));
    int i = key % NBUCKET;
  e->key = key;
  e->value = value;
  pthread_mutex_lock(&bucket_lk[i]);
  e->next = *p;
  *p = e;
  pthread_mutex_unlock(&bucket_lk[i]);
}
  
static 
void put(int key, int value)
{
  int i = key % NBUCKET;

  // is the key already present?
  struct entry *e = 0;

  //pthread_mutex_lock(&bucket_lk[i]);
  //printf("insertin key %d in bucket %d\n",key,i);
  for (e = table[i]; e != 0; e = e->next) {
    
    if (e->key == key)
      break;
  }
//  pthread_mutex_unlock(&bucket_lk[i]);
  if(e){
    // update the existing key.
   // pthread_mutex_lock(&global_lk);
   //printf("found\n");
    e->value = value;
   // pthread_mutex_unlock(&global_lk);
  } else {
    // the new is new.
    
    insert(key, value, &table[i], table[i]);

  }
  
  

}

static struct entry*
get(int key)
{
  int i = key % NBUCKET;


  struct entry *e = 0;
  int cnt=0;
  for (e = table[i]; e != 0; e = e->next) {
  //  pthread_mutex_lock(&global_lk);
    if (e->key == key) break;
 //   pthread_mutex_unlock(&global_lk);
  }
  if(cnt>1)
  printf("key found more than once %d\n",key);
  return e;
}

static void *
put_thread(void *xa)
{
  int n = (int) (long) xa; // thread number
  int b = NKEYS/nthread;

  for (int i = 0; i < b; i++) {
    put(keys[b*n + i], n);
  }

  return NULL;
}

static void *
get_thread(void *xa)
{
  int n = (int) (long) xa; // thread number
  int missing = 0;

  for (int i = 0; i < NKEYS; i++) {
    struct entry *e = get(keys[i]);
    if (e == 0) missing++;
  }
  printf("%d: %d keys missing\n", n, missing);
  return NULL;
}

int
main(int argc, char *argv[])
{
  pthread_t *tha;
  void *value;
  double t1, t0;

  for(int i=0; i<NBUCKET;i++)
  pthread_mutex_init(&bucket_lk[i], NULL);
  if (argc < 2) {
    fprintf(stderr, "Usage: %s nthreads\n", argv[0]);
    exit(-1);
  }
  nthread = atoi(argv[1]);
  tha = malloc(sizeof(pthread_t) * nthread);
  srandom(0);
  assert(NKEYS % nthread == 0);
  for (int i = 0; i < NKEYS; i++) {
    keys[i] = random();
  }

  //
  // first the puts
  //
  t0 = now();
  for(int i = 0; i < nthread; i++) {
    assert(pthread_create(&tha[i], NULL, put_thread, (void *) (long) i) == 0);
  }
  for(int i = 0; i < nthread; i++) {
    assert(pthread_join(tha[i], &value) == 0);
  }
  t1 = now();

  printf("%d puts, %.3f seconds, %.0f puts/second\n",
         NKEYS, t1 - t0, NKEYS / (t1 - t0));

  //
  // now the gets
  //
  t0 = now();
  for(int i = 0; i < nthread; i++) {
    assert(pthread_create(&tha[i], NULL, get_thread, (void *) (long) i) == 0);
  }
  for(int i = 0; i < nthread; i++) {
    assert(pthread_join(tha[i], &value) == 0);
  }
  t1 = now();

  printf("%d gets, %.3f seconds, %.0f gets/second\n",
         NKEYS*nthread, t1 - t0, (NKEYS*nthread) / (t1 - t0));
}

